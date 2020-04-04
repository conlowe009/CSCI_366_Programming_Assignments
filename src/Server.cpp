//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "common.hpp"
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file) {
    /*This stuff here...magic. Overcomplicated and unnecessary magic.
     * Either that or I'm just stupid. Not sure which I'm more scared of.
     */
    file->seekg(0, ios::beg);
    int s = file->tellg();
    file->seekg(0, ios::end);
    int end = file->tellg();
    return end - s;
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    this->board_size = board_size;
    /* Opens up the setup up board streams and throws an error if the
     * files cannot be opened*/
    this->p1_setup_board.open(p1_setup_board);
    if (this->p1_setup_board.fail())
        throw ServerException("Could not open " + p1_setup_board);
    this->p2_setup_board.open(p2_setup_board);
    if (this->p2_setup_board.fail())
        throw ServerException("Could not open " + p2_setup_board);

    /* Check to see if the files are the correct size */
    if(get_file_length(&(this->p1_setup_board)) != (board_size*(board_size+1)))
        throw ServerException("Incorrect board size");
    if(get_file_length(&(this->p2_setup_board)) != (board_size*(board_size+1)))
        throw ServerException("Incorrect board size");
     }


Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    /*Makes sure player number is valid*/
    if (player > MAX_PLAYERS || player < 1)
        throw ServerException("Bad player number");

    /*Test if x or y is out of bounds*/
    if (x >= BOARD_SIZE || y >= BOARD_SIZE) {
        cout << "Coordinate(s) out of bounds\n";
        return OUT_OF_BOUNDS;
    }

    /* Reads in both setup boards into different 2D vector arrays so they can be traversed
     * and it can later be determined if a hit or miss at a specific coordinate */
    string line;
    vector<vector<char>> board;
    ifstream p;

    //Opens the setup board of the opposing player
    if (player == 1)
        p.open("player_2.setup_board.txt");
    else
        p.open("player_1.setup_board.txt");

    /* Reads the setup board of the opposing player into a 2D vector
     * While loop and for loop below was taken from Stackoverflow at:
     * https://stackoverflow.com/questions/56554212/how-do-i-read-a-text-file-into-a-2d-vector
     */
    while (getline(p, line)) {
        vector<char> row;
        for (char &c : line) {
            if (c != '\n') {
                row.push_back(c);
            }
        }
        board.push_back(row);
    }

    //Close the file reader
    p.close();

    /* Determines whether the parameter defined coordinates
     * result in a hit or miss on the opponent's board */
    if (board[y][x] != '_')
        return HIT;
    else
        return MISS;
}


int Server::process_shot(unsigned int player) {
    //Cast player to string. Easier for making file names.
    string p = std::to_string(player);

    //Variables to deserialize shot coordinates into
    int x = 0, y = 0;
    string input_file = "player_" + p + ".shot.json";

    //De-serializing
    ifstream coords_ifp(input_file);
    cereal::JSONInputArchive read_archive(coords_ifp);
    read_archive(x,y);

    //Remove input file for cleanup
    remove(input_file.c_str());
    coords_ifp.close();

    /* Evaluates correct shot with player and coordinates obtained from
     * deserialization and writes to the result file based on the value
     * returned from evaluate_shot
     */
    string file_name = "player_" + p + ".result.json";
    int shot = evaluate_shot(player, x, y);

    //Removes prior instances of shot file
    remove(file_name.c_str());

    if (shot == HIT) {
        ofstream file(file_name);
        cereal::JSONOutputArchive out_archive (file);
        out_archive(cereal::make_nvp("result", HIT));
        return SHOT_FILE_PROCESSED;
    }
    else if (shot == MISS) {
        ofstream file(file_name);
        cereal::JSONOutputArchive out_archive (file);
        out_archive(cereal::make_nvp("result", MISS));
        return SHOT_FILE_PROCESSED;
    }
    else if (shot == OUT_OF_BOUNDS) {
        ofstream file(file_name);
        cereal::JSONOutputArchive out_archive (file);
        out_archive(cereal::make_nvp("result", OUT_OF_BOUNDS));
        return SHOT_FILE_PROCESSED;
    }
   return NO_SHOT_FILE;
}