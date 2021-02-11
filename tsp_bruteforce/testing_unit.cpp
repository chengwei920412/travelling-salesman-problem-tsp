#include "testing_unit.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "timer.h"

testing_unit::testing_unit(string input_file_name, int number_of_repeats, int optimal_solution,
	vector<int> optimal_path)
{
	input_file_name_ = input_file_name;
	number_of_repeats_ = number_of_repeats;
	optimal_solution_ = optimal_solution;
	optimal_path_ = optimal_path;


	ifstream fin(input_file_name);
	if (fin.is_open()) {
		int graph_size;
		fin >> graph_size;

		graph_ = new graph(graph_size);
		graph_->load_from_file(fin);
	}

	fin.close();
}

testing_unit::~testing_unit()
{
	delete graph_;
	
}

void testing_unit::run_tests()
{
	cout << "BRUTEFORCING: ";
	cout << input_file_name_ << endl;

	timer t{};
	for (int i = 0; i < number_of_repeats_; i++)
	{
		t.start();
		bruteforce();
		t.stop();
		times_.push_back(t.elapsed_time());

		cout << "[" << i + 1 << "/" << number_of_repeats_ << "]";
		cout << " Solution correct: " << (solution_ == optimal_solution_ ? "Yes" : "No");
		cout << ". Optimal solution: " << optimal_solution_;
		cout << ". Bruteforce solution: " << solution_;
		cout << ". Time: ";
		cout << fixed << setprecision(9) << times_.back() << endl;
	}
	cout << endl;
	
}

void testing_unit::save_data(ofstream& fout)
{
	fout << input_file_name_ << " ";
	fout << number_of_repeats_ << " ";
	fout << solution_ << " ";
	for(auto vertex : path_)
	{
		fout << vertex << " ";
	}
	
	fout << endl;
	
	for (auto time : times_)
	{
		fout << fixed << setprecision(9) <<  time << endl;
	}
	
}

void testing_unit::bruteforce()
{
	// START
	
	//	Pobierz rozmiar instancji grafu : SIZE.
	//	Utw�rz wektor W liczb ca�kowitych o nast�puj�cej zawarto�ci:
	//	0, 1, 2, 3, ..., SIZE - 1, 0.
	vector<int> vertices;
	vertices.push_back(0);
	for (int i = 1; i < graph_->get_size(); i++)
	{
		vertices.push_back(i);
	}
	vertices.push_back(0);

	// Zmienne pomocnicze do zapami�tania najlepszego rozwi�zania
	// i najlepszej �cie�ki.
	int best_solution = INT_MAX;
	vector<int> best_path;

	do
	{
		// Wykonano wszystkie mozliwe permutacje - NIE.
		// Zmienne pomocnicze do "przejscia" przez graf i zsumowania wagi przejscia.
		int index_a;
		int index_b;
		int current_solution = 0;

		// "Przejd�" przez graf poczynaj�c od wierzcho�ka 0, id�c przez kolejne wierzcho�ki
		//	wskazywane przez kolejne elementy wektora W, ko�cz�c na wierzcho�ku 0.
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			index_a = vertices[i];
			index_b = vertices[i + 1];

			// Zsumuj wag� przej�cia.
			current_solution += graph_->get_value_at(index_a, index_b);
		}

		// Zapami�taj �ciezk� i jej wag�, jezeli jest bardziej optymalna od �ciezek
		// dot�d sprawdzonych.
		if (current_solution < best_solution)
		{
			best_solution = current_solution;
			best_path = vertices;
		}

	// Wykonaj permutacj� wektora wy��czaj�c z permutacji pierwszy i ostatni element.
	// Permutowane s� elemenety ze zbioru : 1, 2, ..., SIZE - 1. 
	} while (next_permutation(vertices.begin() + 1, vertices.end()-1));

	// Wykonano wszystkie mozliwe permutacje - TAK.
	// Zapisz najbardziej optymaln� �ciezk� oraz jej warto��
	solution_ = best_solution;
	path_ = best_path;

	// END
}
