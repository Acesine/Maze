#include <algorithm>
#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

char human = 'R';
char dest = 'D';

int X = 0;
int Y = 0;

class Point {
public:
        int x;
        int y;
        int walls[4];
        int status;
        Point() {
                walls[0] = 1;
                walls[1] = 1;
                walls[2] = 1;
                walls[3] = 1;
        }
};

char getChar() {
        system ("/bin/stty raw");
        char c = getchar();
        system ("/bin/stty cooked");
        return c;
}
void initSys() {
        system("export PS1=\"\"");
        system("clear");
        cout<<"        __  ___                 ____                             "<<endl;
        cout<<"       /  |/  /___ _____  ___  / __ \\__  ______  ____  ___  _____"<<endl;
        cout<<"      / /|_/ / __ `/_  / / _ \\/ /_/ / / / / __ \\/ __ \\/ _ \\/ ___/"<<endl;
        cout<<"     / /  / / /_/ / / /_/  __/ _, _/ /_/ / / / / / / /  __/ /    "<<endl;
        cout<<"    /_/  /_/\\__,_/ /___/\\___/_/ |_|\\__,_/_/ /_/_/ /_/\\___/_/     "<<endl;
}

void clear() {
        system("clear");
}

void initGraph(int height, int width, vector<vector<char> > & img) {
        for (int i=0;i<height;i++) {
                string e = i%2==0? " -" : "| ";
                for (int j=0;j<width;j+=2) {
                        img[i][j] = e[0];
                        img[i][j+1] = e[1];
                }
        }
}

void printGraph(int height, int width, vector<vector<char> > & img) {
        clear();
        for (int i=0;i<height;i++) {
                for (int j=0;j<width;j++) {
                        cout<<img[i][j];
                }
                cout<<endl;
        }
}

bool isValidPosition(int x, int y, int N, int M) {
        return x>=0 && x<N && y>=0 && y<M;
}

/**
Generating a randomized maze, using DFS method from X Y
 - - - -
| | | | |
 - - - -
| | | | |
 - - - -
**/
void generateRandomMaze(int height, int width, vector<vector<char> > & img, int N, int M, vector<vector<Point> > & graph) {
        for (int i=0;i<N;i++) {
                for (int j=0;j<M;j++) {
                        graph[i][j].x = i;
                        graph[i][j].y = j;
                        graph[i][j].status = 0;
                }
        }

        stack<Point*> st;
        graph[X][Y].status = 1;
        st.push(&graph[X][Y]);
        while (!st.empty()) {
                Point *curr = st.top();
                int x = curr->x;
                int y = curr->y;
                //printf("Processing %d %d\n", x, y);
                st.pop();
                vector<Point*> next;
                if (isValidPosition(x-1, y, N, M) && graph[x-1][y].status==0) {
                        next.push_back(&graph[x-1][y]);
                        curr->walls[0] = 0;
                        graph[x-1][y].walls[2] = 0;
                }
                if (isValidPosition(x, y+1, N, M) && graph[x][y+1].status==0) {
                        next.push_back(&graph[x][y+1]);
                        curr->walls[1] = 0;
                        graph[x][y+1].walls[3] = 0;
                }
                if (isValidPosition(x+1, y, N, M) && graph[x+1][y].status==0) {
                        next.push_back(&graph[x+1][y]);
                        curr->walls[2] = 0;
                        graph[x+1][y].walls[0] = 0;
                }
                if (isValidPosition(x, y-1, N, M) && graph[x][y-1].status==0) {
                        next.push_back(&graph[x][y-1]);
                        curr->walls[3] = 0;
                        graph[x][y-1].walls[1] = 0;
                }
                if (next.size() > 0) {
                        random_shuffle(next.begin(), next.end());
                        for (int i=0;i<next.size();i++) {
                                st.push(next[i]);
                                next[i]->status = 1;
                        }
                }
        }

        // modify image
        for (int i=0;i<N;i++) {
                for (int j=0;j<M;j++) {
                        Point *p = &graph[i][j];
                        int _i = 2*i+1;
                        int _j = 2*j+1;
                        if (p->walls[0] == 0) {
                                img[_i-1][_j] = ' ';
                        }
                        if (p->walls[1] == 0) {
                                img[_i][_j+1] = ' ';
                        }
                        if (p->walls[2] == 0) {
                                img[_i+1][_j] = ' ';
                        }
                        if (p->walls[3] == 0) {
                                img[_i][_j-1] = ' ';
                        }
                }
        }

        // after this, X Y are in image index
        X = 2*X + 1;
        Y = 2*Y + 1;
        img[X][Y] = human;
        int _X = rand() % N;
        int _Y = rand() % M;
        img[2*_X+1][2*_Y+1] = dest;
}

bool move(int height, int width, vector<vector<char> > & img, int x, int y) {
        int nextX = X+x;
        int nextY = Y+y;
        if (nextX>=height || nextX<0 || nextY>=width || nextY<0) return false;
        if (img[nextX][nextY] == 'D') return true;
        if (img[nextX][nextY] != ' ') return false;
        img[X][Y] = ' ';
        img[nextX][nextY] = human;
        X = nextX;
        Y = nextY;
        return false;
}

int main(int argc, char *argv[]) {
        initSys();
        int N, M;
        cout<<"[Height Width]: "<<endl;
        cin>>N>>M;
        int height = 2*N+1;
        int width = 2*M+1;

        char c;
        while (true) {
                X = rand() % N;
                Y = rand() % M;
                cout<<"Starting Game..."<<endl;
                vector<vector<char> > img(height, vector<char>(width));
                vector<vector<Point> > graph(N, vector<Point>(M));
                initGraph(height, width, img);
                generateRandomMaze(height, width, img, N, M, graph);
                bool give_up = false;
                while (true) {
                        printGraph(height, width, img);
                        c = getChar();
                        bool result;
                        switch(c) {
                        case 27:
                                cout<<"Bye!"<<endl;
                                return 0;
                        case 'p':
                                cout<<"Given up!"<<endl;
                                give_up = true;
                                break;
                        // up
                        case 'w':
                                result = move(height, width, img, -1, 0);
                                break;
                        // down
                        case 's':
                                result = move(height, width, img, 1, 0);
                                break;
                        // left
                        case 'a':
                                result = move(height, width, img, 0, -1);
                                break;
                        // right
                        case 'd':
                                result = move(height, width, img, 0, 1);
                                break;
                        }
                        if (result || give_up) break;
                }
        }
        return 0;
}
