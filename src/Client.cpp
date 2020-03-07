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
#include "Client.hpp"

Client::~Client() {
    cout << "Clearing action board" << endl;
}


void Client::initialize(unsigned int player, unsigned int board_size){
    //Initialize variables for the Client class
    this->player = player;
    this->board_size = board_size;

    //Vector for action board
    vector<vector<int>> board(board_size, vector<int>(board_size));

    //Create file name based on player and open ofstream
    string p = std::to_string(player);
    std::ofstream init("player_" + p + ".action_board.json");

    //Serializing file to output to JSON file
    cereal::JSONOutputArchive archive(init);

    //Casts vector to name value pair and archives it
    archive(CEREAL_NVP(board));
}


void Client::fire(unsigned int x, unsigned int y) {
    string p = std::to_string(player);
    std::ofstream fire("player_" + p + ".shot.json");
    cereal::JSONOutputArchive archive(fire);
    archive(CEREAL_NVP(x), CEREAL_NVP(y));
}


bool Client::result_available() {
    string file_name = "player_" + std::to_string(this->player) + ".result.json";
    ifstream in_file(file_name);

    //If file can be opened, then return true
    if (in_file)
        return true;
    else
        return false;
}


int Client::get_result() {
    //Create file name based on player from Client class
    string file_name = "player_" + std::to_string(this->player) + ".result.json";

    //Initialize variable for derserialized data
    int result = 0;
    ifstream result_ifp(file_name);
    cereal::JSONInputArchive read_archive(result_ifp);
    read_archive(result);
    remove (file_name.c_str());
    result_ifp.close();

    //Result should never return an int greater than 1
    if (result > 1)
        throw ClientException("Bad result");
    return result;
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    // Deserializes action board into local board
    string file_name = "player_" + std::to_string(this->player) + ".action_board.json";
    vector<vector<int>> board(board_size, vector<int>(board_size));
    ifstream board_ifp(file_name);
    cereal::JSONInputArchive read_archive(board_ifp);
    read_archive(board);
    board_ifp.close();

    //Update local board with result obtained from deserialization
    board[x][y] = result;

    //Reserializes updated board into the same file
    std::ofstream update(file_name);
    cereal::JSONOutputArchive archive(update);
    archive(CEREAL_NVP(board));
}


string Client::render_action_board(){
    //Reads in action board into a locally declared vector
    string file_name = "player_" + std::to_string(this->player) + ".action_board.json";
    vector<vector<int>> board(board_size, vector<int>(board_size));
    ifstream board_ifp(file_name);
    cereal::JSONInputArchive read_archive(board_ifp);
    read_archive(board);
    board_ifp.close();

    //Converts vector board into a single string
    string str;
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            str += std::to_string(board[i][j]) + "\n";
        }
    }

    //Outputs board for viewing
    cout << str;
    return str;
}