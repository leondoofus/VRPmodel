#include "TSP.h"
#include "BP.h"
#include "EvaluateTour.h"

TSP::TSP()
{
}


vector<int> TSP::nearest_neighbor(Graph graph, vector<int> nodes)
{
	vector<int> tour;
	tour.push_back(graph.depot);
	while (nodes.size() > 0)
	{
		int index = 0;
		float vmin = graph.distance(tour[tour.size()-1], nodes[index]);
		for (int it = 0; it != nodes.size(); ++it)
		{
			float v = graph.distance(tour[tour.size() - 1], nodes[it]);
			if  (v < vmin)
			{
				vmin = v;
				index = it;
			}
		}
		tour.push_back(nodes[index]);
		nodes.erase(nodes.begin() + index);
	}
	tour.push_back(graph.depot);
	/*for (int it = 0; it != tour.size(); ++it)
		cout << tour[it] << endl;*/
	return tour;
}

vector<int> TSP::nearest_insertion(Graph graph, vector<int> nodes)
{
	vector<int> tour;
	tour.push_back(graph.depot);
	tour.push_back(graph.depot);
	while (nodes.size() > 0)
	{
		int node = 0;
		float deltaF = graph.distance(tour[0], nodes[node]) + graph.distance(nodes[node], tour[1]) - graph.distance(tour[0], tour[1]);
		int index = 0;
		for (int it = 0; it != nodes.size(); ++it)
		{
			for (int it2 = 0; it2 != tour.size() - 1; ++it2)
			{
				float deltaF2 = graph.distance(tour[it2], nodes[it]) + graph.distance(nodes[it], tour[it2 + 1]) - graph.distance(tour[it2], tour[it2 +1]);
				if (deltaF2 < deltaF)
				{
					node = it;
					index = it2;
				}
			}
		}
		tour.insert(tour.begin() + index + 1, nodes[node]);
		nodes.erase(nodes.begin() + node);
	}
	/*for (int it = 0; it != tour.size(); ++it)
		cout << tour[it] << endl;*/
	return tour;
}

vector<vector<int>> TSP::nearestNeighbor(Graph graph, vector<vector<int>> nodes)
{
	vector<vector<int>> tours;

	for (int i = 0; i != nodes.size(); ++i)
	{
		tours.push_back(nearest_neighbor(graph, nodes[i]));
	}

	for (int it = 0; it != tours.size(); ++it)
	{
		cout << "----------" << endl;
		for (int it2 = 0; it2 != tours[it].size(); ++it2)
			cout << tours[it][it2] << " ";
		cout << endl;
	}
	return tours;
}

vector<vector<int>> TSP::nearestInsertion(Graph graph, vector<vector<int>> nodes)
{
	vector<vector<int>> tours;

	for (int i = 0; i != nodes.size(); ++i)
	{
		tours.push_back(nearest_insertion(graph, nodes[i]));
	}

	// for (int it = 0; it != tours.size(); ++it)
	// {
	// 	cout << "----------" << endl;
	// 	for (int it2 = 0; it2 != tours[it].size(); ++it2)
	// 		cout << tours[it][it2] << " ";
	// 	cout << endl;
	// }
	return tours;
}

vector<int> TSP::swap (int pos1, int pos2, vector<int> vect)
{
	vector<int> res;
	for(int i=0; i< vect.size(); i++){
		if (i == pos1) res.push_back(vect.at(pos2));
		else if (i == pos2) res.push_back(vect.at(pos1));
		else res.push_back(vect.at(i));
	}
	return res;
}

bool TSP::fitnessCondition (vector<float> fitness, int tabuSize)
{
	if (fitness.size() < tabuSize) return false;
	for (int i=1; i<fitness.size(); i++)
		if (fitness.at(i) < fitness.at(0)) return false;
	return true;
}

bool TSP::isEqual(vector<int> v1, vector<int> v2)
{	for (int i=0;i<v1.size();i++)
		if (v1.at(i) != v2.at(i))
			return false;
	return true;
}


vector<int> TSP::getBestNeighbour (Graph graph, vector<int> solution, int tabuSize)
{
	EvaluateTour eval = EvaluateTour();

	vector<vector<int>> tabu;
	vector<float> fitness;

	float bestFitness = eval.evaluate(graph,solution);
	vector<int> bestIndiv = solution;

	tabu.push_back(solution);
	fitness.push_back(bestFitness);

	while (!fitnessCondition(fitness,tabuSize))
	{
		int added = 0;
		for (int i=1; i<solution.size()-2;i++)
		{
			for (int j=i+1; j<solution.size()-1;j++)
			{
				if (i != 1 && j != solution.size()-2)
				{
					vector<int> tmp = swap(i,j,solution);
					bool check = true;
					for (int k = 0; k < tabu.size(); k++)
					{
						if (isEqual(tabu.at(k), tmp))
						{
							check = false;
							break;
						}
					}
					if (check)
					{

						if (added = 0)
						{
							tabu.push_back(tmp);
							float fit = eval.evaluate(graph,tmp);
							fitness.push_back(fit);
							if (fit < bestFitness)
							{
								bestFitness = fit;
								bestIndiv = tmp;
							}
							added ++;
						} else {
							float fit = eval.evaluate(graph,tmp);
							if (fit < fitness.back())
							{
								tabu.pop_back();
								fitness.pop_back();
								tabu.push_back(tmp);
								fitness.push_back(fit);
								added++;
								if (fit < bestFitness)
								{
									bestFitness = fit;
									bestIndiv = tmp;
								}
							}
						}
					}
				}
			}
		}
		if (added == 0) break;
		solution = tabu.back();
	}
	return bestIndiv;
}

vector<vector<int>> TSP::bestNeighbourSearch (Graph graph, vector<vector<int>> solution, int tabuSize)
{
	vector<vector<int>> res;
	for(int i=0; i<solution.size(); i++)
	{
		if (solution.at(i).size() <= 5) {
			res.push_back(solution.at(i));
		}
		else {
			res.push_back(getBestNeighbour(graph, solution.at(i), tabuSize));}
	}
	return res;
}

vector<vector<int>> TSP::localSearch (Graph graph, int tabuSize)
{
	BP bp = BP();
	EvaluateTour eval = EvaluateTour();

	vector<vector<int>> bestSol = nearestInsertion(graph, bp.firstFitDecreasing(graph));
	float bestScore = eval.evaluate(graph, bestSol);

	for (int i=0; i<bestSol.size();i++){
		for(int j=0;j<bestSol.at(i).size();j++){
			cout << bestSol.at(i).at(j) << " ";
		}
		cout << endl;
	}

	cout << "Before " << bestScore << endl;

	vector<vector<int>> newSol = bestNeighbourSearch(graph,bestSol,tabuSize);

	for (int i=0; i<newSol.size();i++){
		for(int j=0;j<newSol.at(i).size();j++){
			cout << newSol.at(i).at(j) << " ";
		}
		cout << endl;
	}
	cout << "After " << eval.evaluate(graph, newSol) << endl;

	return bestSol;
}
