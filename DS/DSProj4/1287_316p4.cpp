// Name: Garrett McDonnell
// ID: 3021287

// NOTE: My program isn't designed to handle txt files that start with V and E, it reads straight as if all lines contain Vi Vj Wij, because the directions didn't mention V and E for this project

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>
#include <list>
#include <ctime>
#include <stack>

#include "1287_316p4.hpp"

using namespace std;

double createMST(std::priority_queue<double, std::vector<double>, std::greater<double> >& min_queue, list<Edge> adjacencyList[], vector<OnOffSwitch>& ViVjWij, vector<Edge>& MST);
void convertFile2Nums(ifstream& file,vector<OnOffSwitch>& ViVjWij);
void createAdjacencyList(const vector<OnOffSwitch> ViVjWij,list<Edge> adjacencyList[]);
int findMax(const vector<OnOffSwitch> ViVjWij);  // Finds maximum of edges (how many edges there are)
void depthFirstTraversal(list<Edge> adjacencyList[], Edge first);

int main()
{
    int menu=-1;
    std::vector<OnOffSwitch> ViVjWij;
    ifstream file;
    file.open("cs316p4.txt");
    convertFile2Nums(file,ViVjWij);
    int max=findMax(ViVjWij);
    list<Edge> adjacencyList[max+1];
    Edge Max;
    Max.edge=max;
    Max.weight=0;
    adjacencyList[0].push_back(Max);  // First element of adjacency linked list set as the largest edge for looping purposes
    createAdjacencyList(ViVjWij,adjacencyList);
    srand(time(0));
    while (menu!=3)
    {
		std::priority_queue<double, std::vector<double>, std::greater<double> > min_queue;
		cout << endl << "Menu:\n (1) Find MST starting at arbitrary vertex\n (2) Depth first traversal starting from your choice of vertex\n (3) Quit" << endl;
        cout << endl << "Your choice: ";
        cin >> menu;
        cout << endl;
        if (menu==1)
        {
			vector<Edge> MST;
			Edge first;
			first.edge=rand()%max;
			while (first.edge<1 || first.edge>max)
			{
				first.edge=rand()%max;
			}
			cout << "Randomly chosen starting edge: " << first.edge << endl << endl;
			first.weight=0;
			MST.push_back(first);
			auto it=adjacencyList[first.edge].begin();
			while (it!=adjacencyList[first.edge].end())
			{
				min_queue.push(it->weight);
				++it;
			}
			double totalCost=createMST(min_queue, adjacencyList, ViVjWij, MST);
			auto it2=MST.begin();
			while (it2!=MST.end())
			{
				if (it2==MST.end()-1)
					cout << it2->edge << endl << endl;
				else
					cout << it2->edge << '-';
				++it2;
			}
			cout << "Total Cost: " << totalCost << endl << endl;
		}
		
		if (menu==2)
		{
			vector<Edge> MST;
			Edge first;
			first.edge=-1;
			while (first.edge<1 || first.edge>max)
			{
				cout << endl << "Please choose starting edge between 1 and " << max << ": ";
				cin >> first.edge;
			}
			first.weight=0;
			depthFirstTraversal(adjacencyList, first);
		}
		
		for (unsigned int i=0; i<ViVjWij.size(); ++i)
		{
			ViVjWij[i].theSwitch=true;
		}
	}
}

void convertFile2Nums(ifstream& file,vector<OnOffSwitch>& ViVjWij)
{	// Converts text file to numbers, capable of handling multi-digit numbers separated by spaces
    string smallString="";
    while (file.is_open() && !file.eof())
    {
        double num=0;
        unsigned int m=0;
        int tally=0;
        double j=0;
        vector<char> charc;
        getline(file,smallString);
        while (m<=smallString.length())
        {
            if (isdigit(smallString[m]))
            {
                ++tally;
                charc.push_back(smallString[m]);
            }
            else
            {
                j=pow(10,(tally-1));
                for (unsigned int k=0; k<charc.size(); k++)
                {
                    num+=(charc[k]-48)*j;
                    j=j/10;
                }
                OnOffSwitch a;
                a.edge=num,a.theSwitch=true;
                if (num!=0)  // ADDED BECAUSE OF READ-IN ERROR, END OF LINE FROM TXT FILE CREATES UNWANTED 0s
					ViVjWij.push_back(a);
                while (!charc.empty())
                {
                    charc.pop_back();
                }
                tally=0;
                j=0;
                num=0;
            }
            ++m;
        }
    }
}

int findMax(const vector<OnOffSwitch> ViVjWij)
{
	int max=-1;
	for (unsigned int x=0; x<ViVjWij.size(); ++x)
	{
		if (((x+1)%3!=0) && ViVjWij[x].edge>max)
		{
			max=(int) ViVjWij[x].edge;
		}
	}
	return max;
}

void createAdjacencyList(const vector<OnOffSwitch> ViVjWij,list<Edge> adjacencyList[])
{
	for (unsigned int x=0; x<ViVjWij.size(); ++x)
	{
		if ((x+1)%3==0)
		{
			Edge one;
			one.edge=(int) ViVjWij[x-2].edge;
			one.weight=ViVjWij[x].edge;
			Edge two;
			two.edge=(int) ViVjWij[x-1].edge;
			two.weight=ViVjWij[x].edge;
			adjacencyList[one.edge].push_back(two);
			adjacencyList[two.edge].push_back(one); 
		}
	}
}

double createMST(std::priority_queue<double, std::vector<double>, std::greater<double> >& min_queue, list<Edge> adjacencyList[], vector<OnOffSwitch>& ViVjWij, vector<Edge>& MST)
{
	double totalCost=0;
	while (!min_queue.empty())
	{
		for (unsigned int x=0; x<MST.size(); ++x)
		{
			for (unsigned int y=0; y<ViVjWij.size(); ++y)
			{
				if ((y+1)%3==0 && (MST[x].edge==ViVjWij[y-2].edge || MST[x].edge==ViVjWij[y-1].edge) && ViVjWij[y].edge==min_queue.top() && (ViVjWij[y-2].theSwitch==true || ViVjWij[y-1].theSwitch==true))
				{
					int tally=0;
					for (unsigned int z=0; z<MST.size(); ++z)
					{
						if (ViVjWij[y-1].edge==MST[z].edge || ViVjWij[y-2].edge==MST[z].edge)
						{
							++tally;
						}
					}
					if (tally==2)
					{
						min_queue.pop();
					}
					else
					{
						Edge newEdge;
						if (MST[x].edge==ViVjWij[y-2].edge)
						{
							newEdge.edge=ViVjWij[y-1].edge;
						}
						else
						{
							newEdge.edge=ViVjWij[y-2].edge;
						}
						newEdge.weight=ViVjWij[y].edge;
						MST.push_back(newEdge);
						ViVjWij[y-1].theSwitch=false;
						ViVjWij[y-2].theSwitch=false;
						totalCost+=newEdge.weight;
						min_queue.pop();
						auto it=adjacencyList[newEdge.edge].begin();
						while (it!=adjacencyList[newEdge.edge].end())
						{
							if (it->weight!=ViVjWij[y].edge)
								min_queue.push(it->weight);
							++it;
						}
					}
				}
			}
		}
	}
	return totalCost;
}

void depthFirstTraversal(list<Edge> adjacencyList[], Edge first)
{
	int totalCost=0;
	vector<int> vec;
	std::stack<int> stack;
	Edge tempEdge=first;
	stack.push(tempEdge.edge);
	vec.push_back(tempEdge.edge);
	while (vec.size()<(unsigned int) adjacencyList[0].begin()->edge)
	{
		tempEdge.edge=stack.top();
		std::priority_queue<double, std::vector<double>, std::greater<double> > min_queue;
		auto it=adjacencyList[tempEdge.edge].begin();
		while (it!=adjacencyList[tempEdge.edge].end())
		{
			min_queue.push(it->weight);
			++it;
		}
		it=adjacencyList[tempEdge.edge].begin();
		while (it!=adjacencyList[tempEdge.edge].end())
		{
			if (it->weight==min_queue.top())
			{
				bool Switch=true;
				for (unsigned int i=0; i<vec.size(); ++i)
				{
					if (vec[i]==it->edge)
					{
						Switch=false;
						if (min_queue.empty())
						{
							stack.pop();
							i=vec.size();
							it=adjacencyList[tempEdge.edge].end();
						}
						else
						{
							min_queue.pop();
							it=adjacencyList[tempEdge.edge].begin();
						}
						
					}
				}
				if (Switch==true)
				{
					totalCost+=it->weight;
					vec.push_back(it->edge);
					stack.push(it->edge);
					while (!min_queue.empty())
						min_queue.pop();
					it=adjacencyList[tempEdge.edge].end();
				}
			}
			else
				++it;
		}
	}
	cout << endl;
	for (unsigned int i=0; i<vec.size(); ++i)
	{
		if (i==vec.size()-1)
			cout << vec[i];
		else
			cout << vec[i] << "->";
	}
	cout << endl << "Total cost (excluding backtracking): " << totalCost << endl;
}
