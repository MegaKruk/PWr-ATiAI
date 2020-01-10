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
		std::cout << "\n5 - Perform parameters experiment";
		std::cout << "\n6 - Make measurements";
		std::cout << "\n0 - Quit\n";
		std::cin >> option;
		switch (option)
		{
		case 1:
		{
			// load data
			std::string filePointer;
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
		    timer->point1 = std::chrono::high_resolution_clock::now();
			greedySearch.solverGreedy(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
			std::cout << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			break;
		}
		case 3:
		{
			// test sa
			SA mySolverSA;
			mySolverSA.paramsInit();
			std::vector<int> firstPath = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			timer->point1 = std::chrono::high_resolution_clock::now();
			mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath, firstItems, noOfCities, noOfItems, knapsack);
			std::cout << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			break;
		}
		case 4:
		{
			// test ga
			GA mySolverGA;
			mySolverGA.paramsInit();
			mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
			timer->point1 = std::chrono::high_resolution_clock::now();
			mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
			std::cout << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			break;
		}
		case 5:
		{
			// parameters experiment
			GreedySearch greedySearch;
			SA mySolverSA;
			GA mySolverGA;

			// greedy for reference
			std::ofstream output1("output/exp1/Greedy_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = greedySearch.solverGreedy(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output1 << timer->countTimeDiff() << "\t" << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output1.close();

			/***************************************************************************************************************/

			// SA Tmax = 1, time limit = 10 sec
			std::ofstream output2("output/exp1/SA_Tmax1_10sec_" + filename + ".log");
			std::vector<int> firstPath1 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems1 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmax(1.0);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath1, firstItems1, noOfCities, noOfItems, knapsack);
				output2 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output2.close();

			// SA Tmax = 10, time limit = 10 sec
			std::ofstream output3("output/exp1/SA_Tmax10_10sec_" + filename + ".log");
			std::vector<int> firstPath2 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems2 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmax(10.0);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath2, firstItems2, noOfCities, noOfItems, knapsack);
				output3 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output3.close();

			// SA Tmax = 100, time limit = 10 sec
			std::ofstream output4("output/exp1/SA_Tmax100_10sec_" + filename + ".log");
			std::vector<int> firstPath3 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems3 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmax(100.0);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath3, firstItems3, noOfCities, noOfItems, knapsack);
				output4 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output4.close();

			// SA Tmax = 1000, time limit = 10 sec
			std::ofstream output5("output/exp1/SA_Tmax1000_10sec_" + filename + ".log");
			std::vector<int> firstPath4 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems4 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmax(1000.0);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath4, firstItems4, noOfCities, noOfItems, knapsack);
				output5 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output5.close();

			/***************************************************************************************************************/
			
			// SA Tmin = 1E-8, time limit = 10 sec
			std::ofstream output6("output/exp1/SA_Tmin1E-8_10sec_" + filename + ".log");
			std::vector<int> firstPath5 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems5 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmin(1E-8);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath5, firstItems5, noOfCities, noOfItems, knapsack);
				output6 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output6.close();

			// SA Tmin = 1E-9, time limit = 10 sec
			std::ofstream output7("output/exp1/SA_Tmin1E-9_10sec_" + filename + ".log");
			std::vector<int> firstPath6 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems6 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmin(1E-9);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath6, firstItems6, noOfCities, noOfItems, knapsack);
				output7 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output7.close();

			// SA Tmin = 1E-10, time limit = 10 sec
			std::ofstream output8("output/exp1/SA_Tmin1E-10_10sec_" + filename + ".log");
			std::vector<int> firstPath7 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems7 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmin(1E-10);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath7, firstItems7, noOfCities, noOfItems, knapsack);
				output8 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output8.close();

			// SA Tmin = 1E-11, time limit = 10 sec
			std::ofstream output9("output/exp1/SA_Tmin1E-11_10sec_" + filename + ".log");
			std::vector<int> firstPath8 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems8 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmin(1E-11);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath8, firstItems8, noOfCities, noOfItems, knapsack);
				output9 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output9.close();

			/***************************************************************************************************************/

			// SA Tcoeff = 0.9, time limit = 10 sec
			std::ofstream output10("output/exp1/SA_Tcoeff0_9_10sec_" + filename + ".log");
			std::vector<int> firstPath9 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems9 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTcoeff(0.9);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath9, firstItems9, noOfCities, noOfItems, knapsack);
				output10 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output10.close();

			// SA Tcoeff = 0.99, time limit = 10 sec
			std::ofstream output11("output/exp1/SA_Tcoeff0_99_10sec_" + filename + ".log");
			std::vector<int> firstPath10 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems10 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTcoeff(0.99);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath10, firstItems10, noOfCities, noOfItems, knapsack);
				output11 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output11.close();

			// SA Tcoeff = 0.995, time limit = 10 sec
			std::ofstream output12("output/exp1/SA_Tcoeff0_995_10sec_" + filename + ".log");
			std::vector<int> firstPath11 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems11 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTcoeff(0.995);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath11, firstItems11, noOfCities, noOfItems, knapsack);
				output12 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output12.close();

			// SA Tcoeff = 0.999, time limit = 10 sec
			std::ofstream output13("output/exp1/SA_Tcoeff0_999_10sec_" + filename + ".log");
			std::vector<int> firstPath12 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems12 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTcoeff(0.999);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath12, firstItems12, noOfCities, noOfItems, knapsack);
				output13 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output13.close();

			/***************************************************************************************************************/

			// GA crossRatio = 50 %, time limit = 10 sec
			std::ofstream output14("output/exp1/GA_crossRatio50_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverRatio(50);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output14 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output14.close();

			// GA crossRatio = 55 %, time limit = 10 sec
			std::ofstream output15("output/exp1/GA_crossRatio55_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverRatio(55);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output15 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output15.close();

			// GA crossRatio = 60 %, time limit = 10 sec
			std::ofstream output16("output/exp1/GA_crossRatio60_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverRatio(60);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output16 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output16.close();

			// GA crossRatio = 65 %, time limit = 10 sec
			std::ofstream output17("output/exp1/GA_crossRatio65_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverRatio(65);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output17 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output17.close();

			/***************************************************************************************************************/

			// GA mutRatio = 0.5 %, time limit = 10 sec
			std::ofstream output18("output/exp1/GA_mutRatio0_5_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setMutationRatio(0.5);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output18 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output18.close();

			// GA mutRatio = 1 %, time limit = 10 sec
			std::ofstream output19("output/exp1/GA_mutRatio1_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setMutationRatio(1);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output19 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output19.close();

			// GA mutRatio = 1.5 %, time limit = 10 sec
			std::ofstream output20("output/exp1/GA_mutRatio1_5_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setMutationRatio(1.5);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output20 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output20.close();

			// GA mutRatio = 2 %, time limit = 10 sec
			std::ofstream output21("output/exp1/GA_mutRatio2_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setMutationRatio(2);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output21 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output21.close();

			/***************************************************************************************************************/

			// GA popSize = 50, time limit = 10 sec
			std::ofstream output22("output/exp1/GA_popSize50_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setPopSize(50);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output22 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output22.close();

			// GA popSize = 75, time limit = 10 sec
			std::ofstream output23("output/exp1/GA_popSize75_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setPopSize(75);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output23 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output23.close();

			// GA popSize = 100, time limit = 10 sec
			std::ofstream output24("output/exp1/GA_popSize100_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setPopSize(100);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output24 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output24.close();

			// GA popSize = 200, time limit = 10 sec
			std::ofstream output25("output/exp1/GA_popSize200_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setPopSize(200);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output25 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output25.close();

			/***************************************************************************************************************/

			// GA selection = tournament, time limit = 10 sec
			std::ofstream output26("output/exp1/GA_selection_tournament_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setSelectionMethod(1);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output26 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output26.close();

			// GA selection = roulette, time limit = 10 sec
			std::ofstream output27("output/exp1/GA_selection_roulette_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setSelectionMethod(2);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output27 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output27.close();

			/***************************************************************************************************************/

			// GA crossover = OPOOX, time limit = 10 sec
			std::ofstream output28("output/exp1/GA_crossover_OPOOX_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverMethod(1);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output28 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output28.close();

			// GA crossover = TPOOX, time limit = 10 sec
			std::ofstream output29("output/exp1/GA_crossover_TPOOX_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverMethod(2);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output29 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output29.close();

			/***************************************************************************************************************/

			break;
		}
		case 6:
		{
			// measurements 
			GreedySearch greedySearch;
			SA mySolverSA;
			GA mySolverGA;

			// greedy for reference
			std::ofstream output30("output/exp2/Greedy_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = greedySearch.solverGreedy(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output30 << timer->countTimeDiff() << "\t" << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output30.close();

			/***************************************************************************************************************/

			// best params for SA from Experiment 1: Tmax = 10, Tmin = 1E-11, Tcoeff = 0.999
			// SA 1 sec
			std::ofstream output31("output/exp2/SA_1sec_" + filename + ".log");
			std::vector<int> firstPath13 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems13 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(1);
			mySolverSA.setTmax(10.0);
			mySolverSA.setTmin(1E-11);
			mySolverSA.setTcoeff(0.999);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath13, firstItems13, noOfCities, noOfItems, knapsack);
				output31 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output31.close();

			// SA 10 sec
			std::ofstream output32("output/exp2/SA_10sec_" + filename + ".log");
			std::vector<int> firstPath14 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems14 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10);
			mySolverSA.setTmax(10.0);
			mySolverSA.setTmin(1E-11);
			mySolverSA.setTcoeff(0.999);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath14, firstItems14, noOfCities, noOfItems, knapsack);
				output32 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output32.close();

			// SA 100 sec
			std::ofstream output33("output/exp2/SA_100sec_" + filename + ".log");
			std::vector<int> firstPath15 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems15 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(100);
			mySolverSA.setTmax(10.0);
			mySolverSA.setTmin(1E-11);
			mySolverSA.setTcoeff(0.999);
			for(int i = 0; i < 10; i++)
			{
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath15, firstItems15, noOfCities, noOfItems, knapsack);
				output33 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output33.close();

			/***************************************************************************************************************/

			// best params for SA from Experiment 1: crossRatio = 55%, mutRatio = 1%, popSize = 200
			// selection = roulette wheel, OX = TPOOX
			// GA 1 sec
			std::ofstream output34("output/exp2/GA_1sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setTimeLimitSec(1);
				mySolverGA.setCrossoverRatio(55);
				mySolverGA.setMutationRatio(1);
				mySolverGA.setPopSize(200);
				mySolverGA.setSelectionMethod(2);
				mySolverGA.setCrossoverMethod(2);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output34 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output34.close();

			// GA 10 sec
			std::ofstream output35("output/exp2/GA_10sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setTimeLimitSec(10);
				mySolverGA.setCrossoverRatio(55);
				mySolverGA.setMutationRatio(1);
				mySolverGA.setPopSize(200);
				mySolverGA.setSelectionMethod(2);
				mySolverGA.setCrossoverMethod(2);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output35 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output35.close();

			// GA 100 sec
			std::ofstream output36("output/exp2/GA_100sec_" + filename + ".log");
			for(int i = 0; i < 10; i++)
			{
				mySolverGA.setTimeLimitSec(100);
				mySolverGA.setCrossoverRatio(55);
				mySolverGA.setMutationRatio(1);
				mySolverGA.setPopSize(200);
				mySolverGA.setSelectionMethod(2);
				mySolverGA.setCrossoverMethod(2);
				mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
				timer->point1 = std::chrono::high_resolution_clock::now();
				float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
				output36 << result << std::endl;
				std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
				std::cout << std::endl << (i + 1) * 100 / 10 << " \% done\n\n";
			}
			output36.close();

			/***************************************************************************************************************/
			// final showdown - 10000 seconds, 1 iteration
			// GA 10000 sec
			/*std::ofstream output37("output/exp2/GA_10000sec_" + filename + ".log");
			mySolverGA.setTimeLimitSec(10000);
			mySolverGA.setCrossoverRatio(55);
			mySolverGA.setMutationRatio(1);
			mySolverGA.setPopSize(200);
			mySolverGA.setSelectionMethod(2);
			mySolverGA.setCrossoverMethod(2);
			mySolverGA.popInit(noOfCities, noOfItems, knapsack, valuableItemsMatrix);
			timer->point1 = std::chrono::high_resolution_clock::now();
			float result = mySolverGA.solverGA(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);
			output37 << result << std::endl;
			std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			output37.close();

			// SA 10000 sec
			std::ofstream output38("output/exp2/SA_10000sec_" + filename + ".log");
			std::vector<int> firstPath16 = mySolverSA.pathInit(noOfCities);
			std::vector<int> firstItems16 = mySolverSA.itemsInit(noOfItems, knapsack, valuableItemsMatrix);
			mySolverSA.setTimeLimitSec(10000);
			mySolverSA.setTmax(10.0);
			mySolverSA.setTmin(1E-11);
			mySolverSA.setTcoeff(0.999);
			timer->point1 = std::chrono::high_resolution_clock::now();
			result = mySolverSA.solverSA(adjacancyMatrix, valuableItemsMatrix, firstPath16, firstItems16, noOfCities, noOfItems, knapsack);
			output38 << result << std::endl;
			std::cout << std::endl << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
			output38.close();*/

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
