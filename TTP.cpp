#include "Stopwatch.h"
#include "TTP.h"
#include "GreedySearch.h"
#include "Knapsack.h"
#include "Item.h"
#include "SA.h"
#include "GA.h"

int TTP::start()
{
	initTTP();
	return 0;
}

int TTP::initTTP()
{
    Item item;
    Knapsack knapsack;
	while (1)
	{
		Stopwatch *timer = new Stopwatch();
		int option;
		std::cout << "\n1 - Load data";
		std::cout << "\n2 - Test Greedy Algorithm";
		std::cout << "\n3 - Test Simulated Annealing";
		std::cout << "\n4 - Test Genetic Algorithm";
		//std::cout << "\n5 - Modify parameters";
		//std::cout << "\n6 - Make measurements for Greedy Algorithm";
		//std::cout << "\n7 - Make measurements for Simulated Annealing";
		//std::cout << "\n8 - Make measurements for Genetic Algorithm";
		std::cout << "\n0 - Quit\n";
		std::cin >> option;
		switch (option)
		{
		case 1:
		{
			// load data
			std::string filename, filePointer;
			std::ifstream myFile;
			std::cout << "Enter filename. Must be in 'data' folder: \n";
			std::cin >> filename;

			timer->point1 = std::chrono::high_resolution_clock::now();

			myFile.open("data/" + filename);
			//myFile.open("D:\\Studia\\Magisterka\\AI\\Project\\PWr-ATiAI\\data\\" + filename);
			if (myFile.is_open())
			{
				do myFile >> filePointer;
				while (filePointer != "DIMENSION:");
				myFile >> filePointer;
				noOfCities = atoi(filePointer.c_str());
				std::cout << "Number of cities:\t" << noOfCities << std::endl;
				adjacancyMatrix.clear();
				adjacancyMatrix.resize(noOfCities);
				for (int i = 0; i < noOfCities; ++i)
					adjacancyMatrix[i].resize(noOfCities);

				do myFile >> filePointer;
				while (filePointer != "ITEMS:");
				myFile >> filePointer;
				noOfItems = atoi(filePointer.c_str());
				std::cout << "Number of items:\t" << noOfItems << std::endl;
				valuableItemsMatrix.clear();
				valuableItemsMatrix.resize(noOfItems);

				do myFile >> filePointer;
				while (filePointer != "KNAPSACK:");
				myFile >> filePointer;
				knapsack.setMaxWeight(atoi(filePointer.c_str()));
				knapsack.setCurrWeight(0);
				//knapsack.getStolenItemsList().clear();
				//knapsack.getStolenItemsList().resize(0);
				std::cout << "Max weight:\t\t" << knapsack.getMaxWeight() << std::endl;

				do myFile >> filePointer;
				while (filePointer != "SPEED:");
				myFile >> filePointer;
				knapsack.setMinSpeed(atof(filePointer.c_str()));
				std::cout << "Min speed:\t\t" << knapsack.getMinSpeed() << std::endl;

				do myFile >> filePointer;
				while (filePointer != "SPEED:");
				myFile >> filePointer;
				knapsack.setMaxSpeed(atof(filePointer.c_str()));
				knapsack.setCurrSpeed();
				std::cout << "Max speed:\t\t" << knapsack.getMaxSpeed() << std::endl;

				do myFile >> filePointer;
				while (filePointer != "RATIO:");
				myFile >> filePointer;
				knapsack.setRentingRatio(atof(filePointer.c_str()));
				std::cout << "Renting ratio:\t\t" << knapsack.getRentingRatio() << std::endl;

				do myFile >> filePointer;
				while (filePointer != "EDGE_WEIGHT_TYPE:");
				myFile >> filePointer;

				if (filePointer == "CEIL_2D")
				{
					std::vector<float> xVect;
					xVect.clear();
					std::vector<float> yVect;
					yVect.clear();

					do myFile >> filePointer;
					while (filePointer != "Y):");

					for (int i = 0; i < noOfCities; i++)
					{
						myFile >> filePointer;
						// !!! couts for debug don't delet pls !!!
						//std::cout << std::endl << filePointer << "\t";

						myFile >> filePointer;
						xVect.push_back(atof(filePointer.c_str()));
						//std::cout << xVect[i] << "\t";

						myFile >> filePointer;
						yVect.push_back(atof(filePointer.c_str()));
						//std::cout << yVect[i] << std::endl;
					}

					std::cout << std::endl << "Adjacancy matrix for cities:\n";
					for (int i = 0; i < noOfCities; i++)
					{
						for (int j = 0; j < noOfCities; j++)
						{
							if (i != j)
							{
								float xDiff = xVect.at(i) - xVect.at(j);
								float yDiff = yVect.at(i) - yVect.at(j);
								float cost = std::nearbyint(sqrt(xDiff * xDiff + yDiff * yDiff));
								adjacancyMatrix[i][j] = cost;
							}
							if (i == j)
							{
								adjacancyMatrix[i][j] = 0;
							}
							std::cout << adjacancyMatrix[i][j] << "\t";
						}
						std::cout << std::endl;
					}
				}
				else
				{
					std::cout << "\nError! Unsupported format.";
					myFile.close();
					break;
				}

				do myFile >> filePointer;
				while (filePointer != "NUMBER):");
				std::cout << "\nValuable items (index, profit, weight, assigned city):";
				for (int i = 0; i < noOfItems; i++)
				{
					myFile >> filePointer;
					valuableItemsMatrix[i].setIdItem(atoi(filePointer.c_str()) - 1);
					std::cout << std::endl << atoi(filePointer.c_str()) - 1 << "\t";

					myFile >> filePointer;
					valuableItemsMatrix[i].setProfit(atoi(filePointer.c_str()));
					std::cout << valuableItemsMatrix[i].getProfit() << "\t";

					myFile >> filePointer;
					valuableItemsMatrix[i].setWeight(atoi(filePointer.c_str()));
					std::cout << valuableItemsMatrix[i].getWeight() << "\t";

					myFile >> filePointer;
					valuableItemsMatrix[i].setAssignedCity(atoi(filePointer.c_str()) - 1);
					std::cout << valuableItemsMatrix[i].getAssignedCity() << "\t";
				}
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			}
			else
			{
				std::cout << "Error! No such file in 'data' directory";
				break;
			}
			break;

		}
		case 2:
		{
			// test greedy
		    GreedySearch greedySearch;
		    std::pair<std::vector<int>, std::vector<int>> result = greedySearch.evaluateGreedy(0, adjacancyMatrix, valuableItemsMatrix, knapsack);
            for (auto i = result.first.begin(); i != result.first.end(); ++i)
                std::cout << *i << ' ';
            std::cout << std::endl;
            for (auto i = result.second.begin(); i != result.second.end(); ++i)
                std::cout << *i << ' ';
			break;
		}
		case 3:
		{
			// test sa
			SA mySolverSA;
			std::vector<int> firstPath = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.paramsInit();

			timer->point1 = std::chrono::high_resolution_clock::now();
			mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath, firstItems, noOfCities, noOfItems, knapsack);
			std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			
			break;
		}
		case 4:
		{
			// test ga
			GA mySolverGA;
			//std::vector<int> firstPath = mySolverGA.pathInit(noOfCities);
			//std::vector<int> items = mySolverGA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
			timer->point1 = std::chrono::high_resolution_clock::now();
			mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
			std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			break;
		}
		case 5:
		{
			// change parameters
			break;
		}
		case 6:
		{
			// measurements greedy
			break;
		}
		case 7:
		{
			// measurements sa
			break;
		}
		case 8:
		{
			// measurements ga
			break;
		}
		case 0:
		{
			adjacancyMatrix.clear();
			return 0;
		}
		default:
		{
			std::cout << "Wrong input";
			break;
		}
		}
	}
}

// getters
int TTP::getNoOfCities()
{
	return noOfCities;
}

int TTP::getNoOfItems()
{
	return noOfItems;
}


std::vector<std::vector<float>> TTP::getAdjacancyMatrix() 
{
	return std::vector<std::vector<float>>(adjacancyMatrix);
}

/*void TTP::setCurrSpeed()
{
	//std::cout << "\ndla cW:" << knapsack.currWeight;
	knapsack.currSpeed = knapsack.minSpeed + (((knapsack.maxWeight - knapsack.currWeight) / knapsack.maxWeight) * (knapsack.maxSpeed - knapsack.minSpeed));
	//std::cout << "\tcS to:" << knapsack.currSpeed;
}*/

TTP::TTP()
{

}

TTP::~TTP()
{

}
