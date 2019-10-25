#include "Stopwatch.h"
#include "TTP.h"

int TTP::start()
{
	initTTP();
	return 0;
}

int TTP::initTTP()
{
	while (1)
	{
		int option;
		std::cout << "\n1 - Load data";
		std::cout << "\n2 - Test Greedy Algorithm";
		std::cout << "\n3 - Test Simulated Annealing (NOT YET READY)";
		std::cout << "\n4 - Test Genetic Algorithm (NOT YET READY)";
		std::cout << "\n5 - Make measurements for Greedy Algorithm";
		std::cout << "\n6 - Make measurements for Simulated Annealing (NOT YET READY)";
		std::cout << "\n7 - Make measurements for Genetic Algorithm (NOT YET READY)";
		std::cout << "\n8 - Modify parameters (NOT YET READY)";
		std::cout << "\n0 - Quit\n";
		std::cin >> option;
		std::cout << std::endl;

		switch (option)
		{
		case 1:
		{
			// load data
			std::string filename, filePointer;
			std::ifstream myFile;
			std::cout << "Enter filename. Must be in 'data' folder: \n";
			std::cin >> filename;

			Stopwatch *timer = new Stopwatch();
			timer->point1 = std::chrono::high_resolution_clock::now();
			
			myFile.open("data/" + filename);
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
				myKnapsack.maxWeight = atoi(filePointer.c_str());
				myKnapsack.currWeight = 0;
				myKnapsack.stolenItemsList.clear();
				myKnapsack.stolenItemsList.resize(0);
				std::cout << "Max weight:\t\t" << myKnapsack.maxWeight << std::endl;

				do myFile >> filePointer;
				while (filePointer != "SPEED:");
				myFile >> filePointer;
				myKnapsack.minSpeed = atof(filePointer.c_str());
				std::cout << "Min speed:\t\t" << myKnapsack.minSpeed << std::endl;

				do myFile >> filePointer;
				while (filePointer != "SPEED:");
				myFile >> filePointer;
				myKnapsack.maxSpeed = atof(filePointer.c_str());
				myKnapsack.currSpeed = myKnapsack.maxSpeed;
				std::cout << "Max speed:\t\t" << myKnapsack.maxSpeed << std::endl;

				do myFile >> filePointer;
				while (filePointer != "RATIO:");
				myFile >> filePointer;
				myKnapsack.rentingRatio = atof(filePointer.c_str());
				std::cout << "Renting ratio:\t\t" << myKnapsack.rentingRatio << std::endl;

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
					//////////////////////////////////////////////////////////////////////////////////////////////////
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
					std::cout << std::endl << atoi(filePointer.c_str()) - 1 << "\t";

					myFile >> filePointer;
					valuableItemsMatrix[i].profit = atoi(filePointer.c_str());
					std::cout << valuableItemsMatrix[i].profit << "\t";

					myFile >> filePointer;
					valuableItemsMatrix[i].weight = atoi(filePointer.c_str());
					std::cout << valuableItemsMatrix[i].weight << "\t";

					myFile >> filePointer;
					valuableItemsMatrix[i].assignedCity = atoi(filePointer.c_str()) - 1;
					std::cout << valuableItemsMatrix[i].assignedCity << "\t";
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
			break;
		}
		case 3:
		{
			// test sa
			break;
		}
		case 4:
		{
			// test ga
			break;
		}
		case 5:
		{
			// measurements greedy
			break;
		}
		case 6:
		{
			// measurements sa
			break;
		}
		case 7:
		{
			// measurements ga
			break;
		}
		case 8:
		{
			// change parameters
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

int TTP::getItemProfit(int index)
{
	return valuableItemsMatrix[index].profit;
}

int TTP::getItemWeight(int index)
{
	return valuableItemsMatrix[index].weight;
}

int TTP::getItemCity(int index)
{
	return valuableItemsMatrix[index].assignedCity;
}

int TTP::getMaxWeight()
{
	return myKnapsack.maxWeight;
}

int TTP::getCurrWeight()
{
	return myKnapsack.currWeight;
}

float TTP::getCurrRentingRatio()
{
	return myKnapsack.rentingRatio;
}

float TTP::getMinSpeed()
{
	return myKnapsack.minSpeed;
}

float TTP::getMaxSpeed()
{
	return myKnapsack.maxSpeed;
}

float TTP::getCurrSpeed()
{
	return myKnapsack.currSpeed;
}

std::vector<int> TTP::getStolenItemsList()
{
	return myKnapsack.stolenItemsList;
}

std::vector<std::vector<float>> TTP::getAdjacancyMatrix()
{
	return std::vector<std::vector<float>>(adjacancyMatrix);
}

void TTP::setCurrWeight(int stolenItemWeight)
{
	myKnapsack.currWeight =+ stolenItemWeight;
}

void TTP::setCurrSpeed()
{
	// uuuh math is hard ;_;
	//currSpeed = 
}

TTP::TTP()
{

}

TTP::~TTP()
{

}
