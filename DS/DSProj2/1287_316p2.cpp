// Name: Garrett McDonnell
// ID: 3021287

// NOTE: For no apparent reason, atleast for the text file I am using, the program works perfectly except when the first line of Vi Vj Wij ends in a multi-digit number, meaning Wij > 9,
//       all subsequent lines of the text file can hold three numbers of any size

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

#include "1287_316p2.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::ifstream;
using std::string;
using std::numeric_limits;
using std::streamsize;
using std::ios;

void shortestPathFinder(vector<OnOffSwitch>& v, vector<Vertex>& Unvisited, vector<Vertex>::iterator& it, Vertex& itVertex);  // Used because Dijkstra's algorithm is "greedy", seeks shortest path at all times
void findAdjacentVertices(vector<OnOffSwitch>& v, vector<Vertex>& Unvisited, Vertex& vert); // Changes information of vertices adjacent to input Vertex
void convertFile2Nums(ifstream& file,vector<OnOffSwitch>& ViVjWij);  // Converts the input text file into a vector of variables of my OnOffSwitch class containing weights and vertices
double convertStr2Num(string s);  // converts a string of numbers separated by spaces into doubles
void recursivePathFinder(vector<OnOffSwitch> ViVjWij, vector<Vertex> vec,Vertex startVertex, const Vertex& endVertex,bool& flag);  // Recursively finds every path from a source vertex to an end vertex
void recursiveDisplay(vector<Vertex> Visited,Vertex vert);  // Recursively displays all paths from start vertex to end vertex by nature of using traveling from end vertex backwards with each recursive call using "prevVertex" of my Vertex class
void reverseRecurse(vector<Vertex> Visited,Vertex vert);  // As far as I could tell, it was impossible to not display an extra "->" after each end vertex using my recursiveDisplay, so this dummy function is used to fix this

int main()
{
    int option=-1,V=-1,E=-1,source;
    string fileName="";
    ifstream file;
    vector<OnOffSwitch> ViVjWij;  // vector where every third element is a weight, preceding two elements are vertices
    vector<Vertex> Visited;  // Once a vertex has been checked and changed all information of adjacent vertices, it is moved from Unvisited to Visited
    vector<Vertex> Unvisited;  // Holds the start vertex as well as any vertex that could be traveled to from the start vertex
    vector<Vertex>::iterator it;
    Vertex itVertex;
    while (option!=4)
    {
        if (file.is_open())
        {  // With each run from the start menu, if the same file is being used, it is easiest for me to read from the beginning of the text file and refill ViVjWij vector with each iteration of my program
            string VE="",s="";  // COPY AND PASTED FROM LATTER PART OF PROGRAM TO FIX SLIGHT ERROR OF OUTPUT
            getline(file,VE);
            int n=0;
            for (unsigned int i=0; i<VE.length(); ++i)
            {
                if (!isdigit(VE[i]) && V==-1)
                { // Takes first line of text file and converts the first part of string into a number for V, number of vertices
                    s=VE.substr(n,i);
                    V=convertStr2Num(s);
                    n=i+1;
                }
            }
            s="";
            s=VE.substr(n,VE.length()); // Rest of string is converted into a number for E, number of edges
            E=convertStr2Num(s);
        }
        cout << "Menu:\n (1) Open Input Files\n (2) Source for Shortest Paths\n (3) Paths from A to B\n (4) Quit" << endl;
        cout << endl << "Your choice: ";
        cin >> option;
        cout << endl;
        if (option==1)
        {
			V=-1;
            if (file.is_open())
            { // After first run of my program, I assume the user wants to use the same text file to find paths until they choose option 1 again, in which case the old file is closed for a new one to be opened
                file.close();
            }
            cout << "Please write the name of the file to take data from: ";
            cin.ignore();
            getline(cin,fileName);
            cout << endl;
            file.open(fileName);
            string VE="",s="";
            getline(file,VE);
            int n=0;
            for (unsigned int i=0; i<VE.length(); ++i)
            {
                if (!isdigit(VE[i]) && V==-1)
                { // Takes first line of text file and converts the first part of string into a number for V, number of vertices
                    s=VE.substr(n,i);
                    V=convertStr2Num(s);
                    n=i+1;
                }
            }
            s="";
            s=VE.substr(n,VE.length()); // Rest of string is converted into a number for E, number of edges
            E=convertStr2Num(s);
            convertFile2Nums(file,ViVjWij);
        }
        else if (option==2)
        {
            if (file.is_open())
            {
                cin.ignore();
                cout << "Enter the source vertex in the range from 1 to " << abs(V) << ": ";
                cin >> source;
                cout << endl;
                Vertex a{static_cast <double> (source)}; // Source vertex created and added to Unvisited
                a.shortestDist=0;
                Unvisited.push_back(a);
                unsigned int z=0;
                while (z<Unvisited.size())
                {  // Loop structure simply finds each unique vertex in ViVjWij that could be traveled to from the source and adds it to Unvisited
					for (unsigned int i=0; i<ViVjWij.size(); i+=3)
					{
						if (ViVjWij[i].vert.thisVertex==Unvisited[z].thisVertex)
						{
							int tally=0;
							for (unsigned int k=0; k<Unvisited.size(); ++k)
							{
								if (ViVjWij[i+1].vert.thisVertex==Unvisited[k].thisVertex)
									++tally;
							}
							if (tally==0)
								Unvisited.push_back(ViVjWij[i+1].vert);
						}
					}
					++z;
				}
				if (Unvisited.size()<=1)
				{ // If Unvisited's size is 1, that means there are no paths leading from source vertex
					cout << "There are no paths leading from Vertex " << source << '.' << endl;
					while (!Unvisited.empty())
					{  // Vector's dynamic memory must be relinquished after each run of program, or else it continues to fill
						Unvisited.pop_back();
					}
				}
				else 
				{
					for (unsigned int i=0; i<Unvisited.size(); ++i)
					{ // Loop structure finds source vertex, updates information of adjacent vertices, adds it to Visited vector and removes it from Unvisited, then moves to next vertex by shortest path
						if (Unvisited[i].thisVertex==source)
						{
							it=Unvisited.begin()+i;
							itVertex=*it;
							findAdjacentVertices(ViVjWij,Unvisited,Unvisited[i]);
							Visited.push_back(Unvisited[i]);
							Unvisited.erase(it);
							shortestPathFinder(ViVjWij,Unvisited,it,itVertex);
							for (unsigned int k=0; k<ViVjWij.size(); ++k)
							{
								if ((k+1)%3!=0 && ViVjWij[k].vert.thisVertex==itVertex.thisVertex)
								{ // Switch is set to false once a vertex has been completely finished, this allows me to ignore completed vertices without tampering with original ViVjWij vector
									ViVjWij[k].theSwitch=false;
								}
							}
							i=Unvisited.size();
						}
					}
					while (!Unvisited.empty())
					{ // Loop structure does same as above, but it was easier to deal with the source vertex separately
/*						
						auto brob=Unvisited.begin();
						while (brob!=Unvisited.end())
						{
							cout << brob->thisVertex << ' ';
							++brob;
						}
						cout << endl;
*/						
						itVertex=*it;
						findAdjacentVertices(ViVjWij,Unvisited,*it);
						Visited.push_back(*it);
						Unvisited.erase(it);
						shortestPathFinder(ViVjWij,Unvisited,it,itVertex);
						for (unsigned int k=0; k<ViVjWij.size(); ++k)
						{
							if ((k+1)%3!=0 && ViVjWij[k].vert.thisVertex==itVertex.thisVertex)
							{
								ViVjWij[k].theSwitch=false;
							}
						}
					}
						auto it=Visited.begin();
						while (it!=Visited.end()-1)
						{
							reverseRecurse(Visited,*(++it));
							cout << endl;
						} 
				}
                cout << endl;
                while (!Visited.empty())
                    Visited.pop_back();
            }
            else
                cout << "Error, no graph has been read in from a file as of yet, returning to main menu. . ." << endl << endl;
        }
        else if (option==3)
        {
            if (file.is_open())
            {
                cout << "Please enter two vertices within the range from 1 to " << abs(V) << ". . .\n";
                cout << "First: ";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                const int start=cin.get()-48;
                cout << endl << "Second: ";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                const int endV=cin.get()-48;
                Vertex startVertex(start);
                startVertex.shortestDist=0;
                Vertex endVertex(endV);
                endVertex.shortestDist=0;
                cout << endl;
                vector<Vertex> newVec;
                bool flag=true;  // Start and end vertex created and used to recursivly find all paths from start to end, end vertex is used as a stopping condition
                recursivePathFinder(ViVjWij,newVec,startVertex,endVertex,flag);
                if (flag)  // Flag is thrown into recursivePathFinder by reference, it is shut off if even one path is found, even if start and end vertices are the same
					cout << "No path from Vertex " << start << " to Vertex " << endV << " exists." << endl << endl;
            }
            else
                cout << "Error, no graph has been read in from a file as of yet, returning to main menu. . ." << endl << endl;
        }
    }
    return 0;
}

void shortestPathFinder(vector<OnOffSwitch>& c, vector<Vertex>& Unvisited, vector<Vertex>::iterator& it,Vertex& itVertex)
{
    double currentShortestDist=std::numeric_limits<double>::infinity();
    unsigned int i=0;
    while (i<c.size())
    {  // Loop structure finds the shortest distance to leave source vertex, as Dijstra's is a greedy algorithm
        if ((i+1)%3==0 && c[i-1].theSwitch==true && c[i-2].theSwitch==true)
        {
            if (c[i-2].vert.thisVertex==itVertex.thisVertex)
            {
                if (c[i].vert.thisVertex<currentShortestDist)
                {
                    currentShortestDist=c[i].vert.thisVertex;
                }
            }
        }
        ++i;
    }
    i=0;
    int vert=-1;
    while (i<c.size())
    {  //  I then loop through original vector to find that shortest distance to the next vertex, and then make a vertex locally of that next vertex
        if ((i+1)%3==0 && (c[i-1].theSwitch==true && c[i-2].theSwitch==true) && c[i].vert.thisVertex==currentShortestDist)
        {
            if (c[i-2].vert.thisVertex==itVertex.thisVertex && c[i-1].vert.thisVertex!=itVertex.thisVertex)
            {
                    vert=c[i-1].vert.thisVertex;
                    i=c.size();
            }
        }
        ++i;
    }
    i=0;
    while (i<Unvisited.size())
    { // I now find the equivalent vertex in Unvisited and point the iterator at that element, as it will now be the next source vertex
        if (Unvisited[i].thisVertex==vert)
        {
            it=Unvisited.begin()+i;
            i=Unvisited.size();
		}
        ++i;
    }
}

void findAdjacentVertices(vector<OnOffSwitch>& c, vector<Vertex>& Unvisited, Vertex& vert)
{
    // Loop through, find the distances in the OnOffSwitch vector, find the corresponding vertices in Unvisited vector,
    // update previous vertex and shortest distance from source, when all adjacent vertices have been visited and updated,
    // the calling vertex can be added to Visited and removed from Unvisited
    Vertex v;
    unsigned int i=0;
    while (i<c.size())
    {
        if ((i+1)%3==0 && c[i-1].theSwitch==true && c[i-2].theSwitch==true)
        {
            if (c[i-2].vert.thisVertex==vert.thisVertex && c[i-1].vert.thisVertex!=vert.thisVertex)
            {
				if ((vert.shortestDist+c[i].vert.thisVertex) < c[i-1].vert.shortestDist)
				{
					c[i-1].vert.shortestDist=vert.shortestDist+c[i].vert.thisVertex;
					c[i-1].vert.prevVertex=vert.thisVertex;
					v=c[i-1].vert;
					unsigned int k=0;
					while (k<c.size())
					{
						if ((k+1)%3!=0 && c[k].vert.thisVertex==v.thisVertex)
						{
							c[k].vert=v;
						}
						++k;
					}
					k=0;
					while (k<Unvisited.size())
					{
						if (Unvisited[k].thisVertex==v.thisVertex)
						{
							Unvisited[k]=v;
							k=Unvisited.size();
						}
						++k;
					}
				}
            }
        }
        ++i;
    }
}

void convertFile2Nums(ifstream& file,vector<OnOffSwitch>& ViVjWij)
{  // Converts text file to numbers, capable of handling multi-digit numbers separated by spaces
	string smallString="";
    while (file.is_open() && !file.eof())
    {
        double num=0;
        unsigned int m=0;
        int tally=0;
        double j=0;
        vector<char> charc;
        getline(file,smallString);
        while (m<=smallString.length())  // SOURCE OF ERROR FROM WHAT I CAN TELL
        {
            if (isdigit(smallString[m]))
            {
                ++tally;
                charc.push_back(smallString[m]);
            }
            else
            {
                j=pow(10,(tally-1));
                for (unsigned int k=0; k<charc.size(); ++k)
                {
					num+=(charc[k]-48)*j;
					j=j/10;
                }
				OnOffSwitch a;
				a.vert.thisVertex=num,a.theSwitch=1;
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

double convertStr2Num(string s)
{ // Converts single string into a number, capable of handling multi digit number
    double num=0;
    unsigned int m=0;
    int tally=0;
    double j=0;
    vector<char> charc;
    while (m<=s.length())
    {
        if (isdigit(s[m]))
        {
            ++tally;
            charc.push_back(s[m]);
        }
        ++m;
    }
    j=pow(10,(tally-1));
    for (unsigned int k=0; k<charc.size(); k++)
    {
        num+=(charc[k]-48)*j;
        j=j/10;
    }
    return num;
}

void recursivePathFinder(vector<OnOffSwitch> ViVjWij, vector<Vertex> vec, Vertex startVertex, const Vertex& endVertex, bool& flag)
{
	unsigned int i=0;
    vec.push_back(startVertex);
    if (startVertex.thisVertex==endVertex.thisVertex)
    { // When stopping condition is found, the weights between vertices are summed, vertices are displayed, and total weight is displayed, vector is then depleted
		flag=false;
        double dist=0;
        while (!vec.empty())
        {
            vector<Vertex>::iterator it=vec.begin();
            dist+=it->shortestDist;
            if (it==vec.end()-1)
                cout << it->thisVertex;
            else
                cout << it->thisVertex << " -> ";
            vec.erase(it);
        }
        cout << endl << "Total weight of path: " << dist << endl << endl;
        return;
    }
    Vertex newVertex;
    while (i<ViVjWij.size())
    { // Until stopping condition is met, I loop through and find adjacent paths to unique vertex (not already in my new vector), and call recursive function again using the new vertex
        if ((i+1)%3==0 && ViVjWij[i].theSwitch==true)
        {
            if ((ViVjWij[i-2].vert.thisVertex==startVertex.thisVertex) && ViVjWij[i-1].vert.thisVertex!=startVertex.thisVertex)
            {
                    int tally=0;
                    for (unsigned int k=0; k<vec.size(); ++k)
                    {
                        if (vec[k].thisVertex==ViVjWij[i-1].vert.thisVertex)
                        {
                            ++tally;
                        }
                    }
                    if (tally==0)
                    {
                        newVertex.thisVertex=ViVjWij[i-1].vert.thisVertex;
                        newVertex.shortestDist=ViVjWij[i].vert.thisVertex;
                        newVertex.prevVertex=ViVjWij[i-2].vert.thisVertex;
                        ViVjWij[i].theSwitch=false;
                        recursivePathFinder(ViVjWij,vec,newVertex,endVertex,flag);
                    }
            }
        }
        ++i;
    }
}

void recursiveDisplay(vector<Vertex> Visited,Vertex vert)
{ // Finds each visited vertex, recursively travels back each prevVertex, function displays vertex at the end, so last call to recursiveDisplay will display start vertex
	unsigned int i=0;
	while (i<Visited.size())
	{
		if (Visited[i].thisVertex==vert.thisVertex && Visited[i].prevVertex>0)
		{
			unsigned int j=0;
			while (j<Visited.size())
			{
				if (Visited[j].thisVertex==Visited[i].prevVertex)
				{
					recursiveDisplay(Visited,Visited[j]);
					j=Visited.size();
				}
				++j;
			}
			i=Visited.size();
		}
		++i;
	}
	cout << vert.thisVertex << "->";
}

void reverseRecurse(vector<Vertex> Visited,Vertex vert)
{ // Function needed only because of the nature of recursively traveling backward to display vertices, I did not want "->" displayed after the last vertex in the path, so this function is used
	Vertex dummy{static_cast <double> (vert.prevVertex)};
	recursiveDisplay(Visited,dummy);
	cout << vert.thisVertex << endl << "Total weight of path: " << vert.shortestDist << endl;
}
