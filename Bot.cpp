#include "Bot.h"
#include "Piece.h"

#define EMPTY '-'

#include <bits/stdc++.h>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

const string Bot::BOT_NAME = "Bongcloud"; /* Edit this, escaped characters are forbidden */
extern PlaySide getEngineSide(); //

Bot::Bot() { /* Initialize custom fields here */

    board = {
          {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
          {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
          {'-', '-', '-', '-', '-', '-', '-', '-'},
          {'-', '-', '-', '-', '-', '-', '-', '-'},
          {'-', '-', '-', '-', '-', '-', '-', '-'},
          {'-', '-', '-', '-', '-', '-', '-', '-'},
          {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
          {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    WCastleKSide = true;
    WCastleQSide = true;

    BCastleKSide = true;
    BCastleQSide = true;

    movesWithoutCaptureOrPawnMove = 0;

    iwhiteKing = 0;
    jwhiteKing = 4;

    iblackKing = 7;
    jblackKing = 4;

    lastMove = NULL;
}

int Bot::getRow(string pos) {
  return pos.at(1) - 49;
}

int Bot::getCol(string pos) {
  return pos.at(0) - 97;
}

string Bot::getPos(int i, int j) {
  string pos = "";
  pos += (char)(j + 97);
  pos += (char)(i + 49);
  return pos;
}

bool Bot::isEnemyPiece(char piece, char enemy) {
  
  if (piece > 97) {
    switch (enemy) {
      case 'R':
      case 'N':
      case 'B':
      case 'Q':
      case 'K':
      case 'P': return true;
      default : return false;
    }
  } else if (piece < 97) {
    switch (enemy) {
      case 'r':
      case 'n':
      case 'b':
      case 'q':
      case 'k':
      case 'p': return true;
      default : return false;
    }
  }

  return false;
}

bool Bot::isMyPiece(char piece, char myPiece) {
  if (piece > 97) {
    switch (myPiece) {
      case 'r':
      case 'n':
      case 'b':
      case 'q':
      case 'k':
      case 'p': return true;
      default : return false;
    }
  } else if (piece < 97) {
      switch (myPiece) {
        case 'R':
        case 'N':
        case 'B':
        case 'Q':
        case 'K':
        case 'P': return true;
        default : return false;
      }
  }

  return false;
}

bool Bot::isFreeSquare(char square) {
  if (square == EMPTY) {
    return true;
  }

  return false;
}

vector<Move*> Bot::getPawnMoves(string source, PlaySide mySide) {
  vector<Move*> moves;

  int row = getRow(source);  
  int col = getCol(source);
  char piece = board[row][col];
  string dest;

  if (mySide == BLACK) {
    if (row == 6) {
      if (board[row - 1][col] == EMPTY && board[row - 2][col] == EMPTY) {
        dest = getPos(row - 2, col);
        moves.push_back(Move::moveTo(source, dest));
      }
    } else if (row == 1) {

      if (col >= 0 && col < 6) {
        if (isEnemyPiece(board[row][col], board[row - 1][col + 1])) {
          dest = getPos(row - 1, col + 1);
          for (int i = 1; i <= 4; ++i) {
            moves.push_back(Move::promote(source, dest, (Piece)i));
          }
        }
      }

      if (col >= 1 && col < 8) {
        if (isEnemyPiece(board[row][col], board[row - 1][col - 1])) {
          dest = getPos(row - 1, col - 1);
          for (int i = 1; i <= 4; ++i) {
            moves.push_back(Move::promote(source, dest, (Piece)i));
          }
        }
      }

      if (board[row - 1][col] == EMPTY) {
          dest = getPos(row - 1, col);
          for (int i = 1; i <= 4; ++i) {
            moves.push_back(Move::promote(source, dest, (Piece)i));
        }
      }

      return moves;
    }

    if (board[row - 1][col] == EMPTY) {
        dest = getPos(row - 1, col);
        moves.push_back(Move::moveTo(source, dest));
    }

    if (col >= 1 && col <= 6) {
        if (isEnemyPiece(piece, board[row - 1][col - 1])) {
          dest = getPos(row - 1, col - 1);
          moves.push_back(Move::moveTo(source, dest));
        }

        if (isEnemyPiece(piece, board[row - 1][col + 1])) {
          dest = getPos(row - 1, col + 1);
          moves.push_back(Move::moveTo(source, dest));
        }

        // EN PASSANT

        if (board[row][col - 1] == 'p' && lastMove->destination.value() == getPos(row, col - 1)
               && getRow(lastMove->destination.value()) - getRow(lastMove->source.value()) == 2) {
            dest = getPos(row + 1, col -1 );
            moves.push_back(Move::moveTo(source, dest));
        }

        if (board[row][col + 1] == 'p' && lastMove->destination.value() == getPos(row, col + 1)
              && getRow(lastMove->destination.value()) - getRow(lastMove->source.value()) == 2) {
            dest = getPos(row + 1, col + 1);
            moves.push_back(Move::moveTo(source, dest));
        }
    } else if (col == 0) {
      if (isEnemyPiece(piece, board[row - 1][col + 1])) {
          dest = getPos(row - 1, col + 1);
          moves.push_back(Move::moveTo(source, dest));
      }

      // EN PASSANT

      if (board[row][col + 1] == 'p' && lastMove->destination.value() == getPos(row, col + 1)
              && getRow(lastMove->destination.value()) - getRow(lastMove->source.value()) == 2) {
            dest = getPos(row + 1, col + 1);
            moves.push_back(Move::moveTo(source, dest));
      }
    } else if (col == 7) {
      if (isEnemyPiece(piece, board[row - 1][col - 1])) {
          dest = getPos(row - 1, col - 1);
          moves.push_back(Move::moveTo(source, dest));
      }

      // EN PASSANT

      if (board[row][col - 1] == 'p' && lastMove->destination.value() == getPos(row, col - 1)
               && getRow(lastMove->destination.value()) - getRow(lastMove->source.value()) == 2) {
            dest = getPos(row + 1, col -1 );
            moves.push_back(Move::moveTo(source, dest));
      }
    }
  } else if (mySide == WHITE) {
    if (row == 1) {
      if (board[row + 1][col] == EMPTY && board[row + 2][col] == EMPTY) {
        dest = getPos(row + 2, col);
        moves.push_back(Move::moveTo(source, dest));
      }
    } else if (row == 6) {

      // generate promote moves

        if (col >= 0 && col < 6) {
        if (isEnemyPiece(board[row][col], board[row + 1][col + 1])) {
          dest = getPos(row + 1, col + 1);
          for (int i = 1; i <= 4; ++i) {
            moves.push_back(Move::promote(source, dest, (Piece)i));
          }
        }
      }

      if (col >= 1 && col < 8) {
        if (isEnemyPiece(board[row][col], board[row + 1][col - 1])) {
          dest = getPos(row + 1, col - 1);
          for (int i = 1; i <= 4; ++i) {
            moves.push_back(Move::promote(source, dest, (Piece)i));
          }
        }
      }

      if (board[row + 1][col] == EMPTY) {
          dest = getPos(row + 1, col);
          for (int i = 1; i <= 4; ++i) {
            moves.push_back(Move::promote(source, dest, (Piece)i));
        }
      }

      return moves;


    }

    if (board[row + 1][col] == EMPTY) {
      dest = getPos(row + 1, col);
      moves.push_back(Move::moveTo(source, dest));
    }

    if (col >= 1 && col <= 6) {
      if (isEnemyPiece(piece, board[row + 1][col - 1])) {
        dest = getPos(row + 1, col - 1);
        moves.push_back(Move::moveTo(source, dest));
      }

      if (isEnemyPiece(piece, board[row + 1][col + 1])) {
        dest = getPos(row + 1, col + 1);
        moves.push_back(Move::moveTo(source, dest));
      }

      // EN PASSANT

      if (board[row][col - 1] == 'P' && lastMove->destination.value() == getPos(row, col - 1)
               && getRow(lastMove->source.value()) - getRow(lastMove->destination.value()) == 2) {
            dest = getPos(row - 1, col -1);
            moves.push_back(Move::moveTo(source, dest));
        }

      if (board[row][col + 1] == 'P' && lastMove->destination.value() == getPos(row, col + 1)
              && getRow(lastMove->source.value()) - getRow(lastMove->destination.value()) == 2) {
            dest = getPos(row - 1, col + 1);
            moves.push_back(Move::moveTo(source, dest));
      }

    } else if (col == 0) {
      if (isEnemyPiece(piece, board[row + 1][col + 1])) {
        dest = getPos(row + 1, col + 1);
        moves.push_back(Move::moveTo(source, dest));
      }

      // EN PASSANT

      if (board[row][col + 1] == 'P' && lastMove->destination.value() == getPos(row, col + 1)
              && getRow(lastMove->source.value()) - getRow(lastMove->destination.value()) == 2) {
            dest = getPos(row - 1, col + 1);
            moves.push_back(Move::moveTo(source, dest));
      }
    } else if (col == 7) {
      if (isEnemyPiece(piece, board[row + 1][col - 1])) {
        dest = getPos(row + 1, col - 1);
        moves.push_back(Move::moveTo(source, dest));
      }

      // EN PASSANT

      if (board[row][col - 1] == 'P' && lastMove->destination.value() == getPos(row, col - 1)
               && getRow(lastMove->source.value()) - getRow(lastMove->destination.value()) == 2) {
            dest = getPos(row - 1, col -1);
            moves.push_back(Move::moveTo(source, dest));
        }
    }
  }

  return moves;
}

vector<Move*> Bot::getKnightMoves(string source) {
  vector<Move*> moves;
  int i = getRow(source);
  int j = getCol(source);
  int currI = 0, currJ = 0;
  string dest;

  int di[] = {-2, -2, -1, -1, 1, 1, 2, 2};
  int dj[] = {-1, 1, -2, 2, -2, 2, -1, 1};

  for (int k = 0; k < 8; ++k) {
    currI = i + di[k];
    currJ = j + dj[k];

    if (currI >= 0 && currI < 8 && currJ >= 0 && currJ < 8) {
      if (isEnemyPiece(board[i][j], board[currI][currJ]) || board[currI][currJ] == EMPTY) {
        dest = getPos(currI, currJ);
        moves.push_back(Move::moveTo(source, dest));
      }
    }
  }

  return moves;
}

vector<Move*> Bot::getKingMoves(string source) {
  vector<Move*> moves;
  int i = getRow(source);
  int j = getCol(source);
  string dest;

  PlaySide playSide = (board[i][j] == 'k') ? WHITE : BLACK;

  int di[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int dj[] = {-1, -1, -1, 0, 1, 1, 1, 0};

  int currI = 0, currJ = 0;

  for (int k = 0; k < 8; ++k) {
    currI = i + di[k];
    currJ = j + dj[k];

    if (currI >= 0 && currI < 8 && currJ >= 0 && currJ < 8) {
        if (!isMyPiece(board[i][j], board[currI][currJ]) && isSquareAttacked(playSide, getPos(currI, currJ)).size() == 0) {
          dest = getPos(currI, currJ);
          moves.push_back(Move::moveTo(source, dest));
        }
    }
  }

  return moves;
}

vector<Move*> Bot::getRookMoves(string source) {
  vector<Move*> moves;
  int i = getRow(source);
  int j = getCol(source);
  int k = 0;

  string dest;

  for (k = i; k < 8; ++k) {
    if (k == i) {
      continue;
    }
    if (board[k][j] == EMPTY) {
      dest = getPos(k, j);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[k][j])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[k][j])) {
      dest = getPos(k, j);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  for (k = i; k >= 0; --k) {
    if (k == i) {
      continue;
    }
    if (board[k][j] == EMPTY) {
      dest = getPos(k, j);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[k][j])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[k][j])) {
      dest = getPos(k, j);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  for (k = j; k < 8; ++k) {
    if (k == j) {
      continue;
    }
    if (board[i][k] == EMPTY) {
      dest = getPos(i, k);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[i][k])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[i][k])) {
      dest = getPos(i, k);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  for (k = j; k >= 0; --k) {
    if (k == j) {
      continue;
    }
    if (board[i][k] == EMPTY) {
      dest = getPos(i, k);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[i][k])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[i][k])) {
      dest = getPos(i, k);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }
  
  return moves;
}

vector<Move*> Bot::getBishopMoves(string source) {
  vector<Move*> moves;
  int i = getRow(source);
  int j = getCol(source);
  int row = 0, col = 0;

  string dest;

  for (row = i, col = j; row < 8 && col < 8; ++row, ++col) {
    if (row == i && col == j) {
      continue;
    }
    if (board[row][col] == EMPTY) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[row][col])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[row][col])) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  for (row = i, col = j; row >= 0 && col >= 0; --row, --col) {
    if (row == i && col == j) {
      continue;
    }
    if (board[row][col] == EMPTY) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[row][col])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[row][col])) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  for (row = i, col = j; row >= 0 && col > 8; --row, ++col) {
    if (row == i && col == j) {
      continue;
    }
    if (board[row][col] == EMPTY) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[row][col])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[row][col])) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  for (row = i, col = j; row > 8 && col >= 0; ++row, --col) {
    if (row == i && col == j) {
      continue;
    }
    if (board[row][col] == EMPTY) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
    } else if (isMyPiece(board[i][j], board[row][col])) {
      break;
    } else if (isEnemyPiece(board[i][j], board[row][col])) {
      dest = getPos(row, col);
      moves.push_back(Move::moveTo(source, dest));
      break;
    }
  }

  return moves;
}

vector<Move*> Bot::getQueenMoves(string source) {
  vector<Move*> moves1 = getBishopMoves(source);
  vector<Move*> moves2 = getRookMoves(source);
  moves1.insert(moves1.end(), moves2.begin(), moves2.end());
  return moves1;
}

vector<Move*> Bot:: getAllMoves(PlaySide playSide) {
  vector<Move*> moves;
  vector<Move*> tempMoves;
  string source;
  if (playSide == WHITE) {
    for (int i = 0; i < SIZE; ++i) {
      for (int j = 0; j < SIZE; ++j) {
        switch (board[i][j]) {
          case 'r': 
              source = getPos(i, j);
              tempMoves = getRookMoves(source);
              moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
              break;
          case 'n':
              source = getPos(i, j);
              tempMoves = getKnightMoves(source);
              moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
              break;
          case 'b':
              source = getPos(i, j);
              tempMoves = getBishopMoves(source);
              moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
              break;
          case 'q':
              source = getPos(i, j);
              tempMoves = getQueenMoves(source);
              moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
              break;
          case 'k':
              source = getPos(i, j);
              tempMoves = getKingMoves(source);
              moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
              break;
          case 'p':
              source = getPos(i, j);
              tempMoves = getPawnMoves(source, playSide);
              moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
              break;
        }
      }
    }
  } else if (playSide == BLACK) {
      for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
          switch (board[i][j]) {
            case 'R': 
                // source = getPos(i, j);
                // tempMoves = getRookMoves(source);
                // moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());/
                break;
            case 'N':
                source = getPos(i, j);
                tempMoves = getKnightMoves(source);
                moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
                break;
            case 'B':
                source = getPos(i, j);
                tempMoves = getBishopMoves(source);
                moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
                break;
            case 'Q':
                source = getPos(i, j);
                tempMoves = getQueenMoves(source);
                moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
                break;
            case 'K':
                // source = getPos(i, j); 
                // tempMoves = getKingMoves(source);
                // moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
                break;
            case 'P':
                source = getPos(i, j);
                tempMoves = getPawnMoves(source, playSide);
                moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
                break;
          }
        }
      }
    }
    return moves;
}

void Bot::deallocateVector(vector<Move*> &moves) {

  for (unsigned int i = 0; i < moves.size(); ++i) {
    free(moves[i]);
  }

  moves.clear();
}

vector<string> Bot::isSquareAttacked(PlaySide playSide, string src) {
    
  vector<string> squares;
  int row = getRow(src);
  int col = getCol(src);
  int i, j, k;

  int diKnight[] = {-2, -2, -1, -1, 1, 1, 2, 2};
  int djKnight[] = {-1, 1, -2, 2, -2, 2, -1, 1};

  int diKing[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int djKing[] = {-1, -1, -1, 0, 1, 1, 1, 0};

  if (playSide == WHITE) {
    
    // pawn check
    if (row < 7 && col > 0 && col < 7) {
      if (board[row + 1][col - 1] == 'P') {
        squares.push_back(getPos(row + 1, col - 1));
      }


      if (board[row + 1][col + 1] == 'P') {
        squares.push_back(getPos(row + 1, col + 1));
      }
    }

    //  knight check
    for (k = 0; k < 8; ++k) {
      i = row + diKnight[k];
      j = col + djKnight[k];

      if (i >= 0 && i < 8 && j < 8 && j >= 0) {
        if (board[i][j] == 'N') {
          squares.push_back(getPos(i, j));
        }
      }
    }

    // king check
    for (k = 0; k < 8; ++k) {
      i = row + diKing[k];
      j = col + djKing[k];

      if (i >= 0 && i < 8 && j < 8 && j >= 0) {
        if (board[i][j] == 'K') {
          squares.push_back(getPos(i, j));
        }
      }
    }


    // diagonal check
    for (i = row + 1, j = col - 1; i < 8 && j >= 0; ++i, --j) {
        if (board[i][j] == 'Q' || board[i][j] == 'B') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'k') {
          continue; 
        } else if (isMyPiece('k', board[i][j])) {
          break;
        }
    }

    for (i = row + 1, j = col + 1; i < 8 && j < 8; ++i, ++j) {
        if (board[i][j] == 'Q' || board[i][j] == 'B') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'k') {
          continue; 
        } else if (isMyPiece('k', board[i][j])) {
          break;
        }
    }

    for (i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 'Q' || board[i][j] == 'B') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'k') {
          continue; 
        } else if (isMyPiece('k', board[i][j])) {
          break;
        }
    }

    for (i = row - 1, j = col + 1; i >= 0 && j < 8; --i, ++j) {
        if (board[i][j] == 'Q' || board[i][j] == 'B') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'k') {
          continue; 
        } else if (isMyPiece('k', board[i][j])) {
          break;
        }
    }

    //vertical check

    for (i = row + 1; i < 8; ++i) {
      if (board[i][col] == 'Q' || board[i][col] == 'R') {
        squares.push_back(getPos(i, col));
      } else if (board[i][col] == 'k') {
          continue; 
      } else if (isMyPiece('k', board[i][col])) {
        break;
      }
    }

    for (i = row - 1; i >= 0; --i) {
      if (board[i][col] == 'Q' || board[i][col] == 'R') {
        squares.push_back(getPos(i, col));
      } else if (board[i][col] == 'k') {
          continue; 
      } else if (isMyPiece('k', board[i][col])) {
        break;
      }
    }

    //orizontal check

    for (j = col + 1; j < 8; ++j) {
      if (board[row][j] == 'Q' || board[row][j] == 'R') {
        squares.push_back(getPos(row, j));
      } else if (board[row][j] == 'k') {
          continue; 
      } else if (isMyPiece('k', board[row][j])) {
        break;
      }
    } 

    for (j = col - 1; j >= 0; --j) {
      if (board[row][j] == 'Q' || board[row][j] == 'R') {
        squares.push_back(getPos(row, j));
      } else if (board[row][j] == 'k') {
          continue; 
      } else if (isMyPiece('k', board[row][j])) {
        break;
      }
    } 

  } else if (playSide == BLACK) {

      // pawn check
    if (row > 0 && col > 0 && col < 7) {
      if (board[row - 1][col - 1] == 'p') {
        squares.push_back(getPos(row - 1, col - 1));
      }


      if (board[row - 1][col + 1] == 'p') {
        squares.push_back(getPos(row - 1, col + 1));
      }
    }

      


    //  knight check
    for (k = 0; k < 8; ++k) {
      i = row + diKnight[k];
      j = col + djKnight[k];
      
      if (i >= 0 && i < 8 && j < 8 && j >= 0) {
        if (board[i][j] == 'n') {
          squares.push_back(getPos(i, j));
        }
      }
      
    }

    // king check
    for (k = 0; k < 8; ++k) {
      i = row + diKing[k];
      j = col + djKing[k];

      if (i >= 0 && i < 8 && j < 8 && j >= 0) {
        if (board[i][j] == 'k') {
          squares.push_back(getPos(i, j));
        }
      }
    }


    // diagonal check
    for (i = row + 1, j = col - 1; i < 8 && j >= 0; ++i, --j) {
        if (board[i][j] == 'q' || board[i][j] == 'b') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'K') {
          continue;
        } else if (isMyPiece('K', board[i][j])) {
          break;
        }
    }

    for (i = row + 1, j = col + 1; i < 8 && j < 8; ++i, ++j) {
        if (board[i][j] == 'q' || board[i][j] == 'b') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'K') {
          continue;
        } else if (isMyPiece('K', board[i][j])) {
          break;
        }
    }

    for (i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 'q' || board[i][j] == 'b') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'K') {
          continue;
        } else if (isMyPiece('K', board[i][j])) {
          break;
        }
    }

    for (i = row - 1, j = col + 1; i >= 0 && j < 8; --i, ++j) {
        if (board[i][j] == 'q' || board[i][j] == 'b') {
          squares.push_back(getPos(i, j));
        } else if (board[i][j] == 'K') {
          continue;
        } else if (isMyPiece('K', board[i][j])) {
          break;
        }
    }

    //vertical check

    for (i = row + 1; i < 8; ++i) {
      if (board[i][col] == 'q' || board[i][col] == 'r') {
        squares.push_back(getPos(i, col));
      } else if (board[i][col] == 'K') {
          continue;
      } else if (isMyPiece('K', board[i][col])) {
        break;
      }
    }

    for (i = row - 1; i >= 0; --i) {
      if (board[i][col] == 'q' || board[i][col] == 'r') {
        squares.push_back(getPos(i, col));
      } else if (board[i][col] == 'K') {
          continue;
      } else if (isMyPiece('K', board[i][col])) {
        break;
      }
    }

    //orizontal check

    for (j = col + 1; j < 8; ++j) {
      if (board[row][j] == 'q' || board[row][j] == 'r') {
        squares.push_back(getPos(row, j));
      } else if (board[row][j] == 'K') {
          continue;
      } else if (isMyPiece('K', board[row][j])) {
        break;
      }
    } 

    for (j = col - 1; j >= 0; --j) {
      if (board[row][j] == 'q' || board[row][j] == 'r') {
        squares.push_back(getPos(row, j));
      } else if (board[row][j] == 'K') {
          continue;
      } else if (isMyPiece('K', board[row][j])) {
        break;
      }
    } 
  }

  return squares;
}

bool Bot::canCastleKSide(PlaySide playSide) {

  if (playSide == WHITE) {
    if (WCastleKSide == false) {
      return false;
    }

    if (isSquareAttacked(playSide, getPos(iwhiteKing, jwhiteKing)).size() != 0) {
      return false;
    }

    for (int j = jwhiteKing + 1; j < 7; ++j) {
      if (isSquareAttacked(playSide, getPos(iwhiteKing, j)).size() != 0 || board[iwhiteKing][j] != EMPTY) {
        return false;
      }
    }

    return true;
  } else if (playSide == BLACK) {
    if (BCastleKSide == false) {
      return false;
    }

    if (isSquareAttacked(playSide, getPos(iblackKing, jblackKing)).size() != 0) {
      return false;
    }

    for (int j = jblackKing; j < 7; ++j) {
      if (isSquareAttacked(playSide, getPos(iblackKing, j)).size() != 0 || board[iblackKing][j] != EMPTY) {
        return false;
      }
    }

    return true;
  }

  return false;
}

bool Bot::canCastleQSide(PlaySide playSide) {
  
  if (playSide == WHITE) {

    if (WCastleQSide == false) {
      return false;
    }

    if (isSquareAttacked(playSide, getPos(iwhiteKing, jwhiteKing)).size() != 0) {
      return false;
    }

    for (int j = jwhiteKing - 1; j >= 2; ++j) {
      if (isSquareAttacked(playSide, getPos(iwhiteKing, j)).size() != 0 || board[iwhiteKing][j] != EMPTY) {
        return false;
      }
    }

    return true;

  } else if (playSide == BLACK) {

    if (BCastleQSide == false) {
      return false;
    }

    if (isSquareAttacked(playSide, getPos(iblackKing, jblackKing)).size() != 0) {
      return false;
    }

    for (int j = jblackKing - 1; j >= 2; --j) {
      if (isSquareAttacked(playSide, getPos(iblackKing, j)).size() != 0 || board[iblackKing][j] != EMPTY) {
        return false;
      }
    }

    return true;

  }

  return false;
}

void Bot::recordMove(Move* move, PlaySide sideToMove) {
    /* You might find it useful to also separately
     * record last move in another custom field */

    int iSrc = getRow(move->source.value());
    int jSrc = getCol(move->source.value());

    int iDest = getRow(move->destination.value());
    int jDest = getCol(move->destination.value());

    if (move->source.value() == getPos(0, 0)) {
      WCastleQSide = false;
    } else if (move->source.value() == getPos(0, 7)) {
      WCastleKSide = false;
    } else if (move->source.value() == getPos(7, 0)) {
      BCastleQSide = false;
    } else if (move->source.value() == getPos(7, 7)) {
      BCastleKSide = false;
    }

    if (move->source.value() == getPos(0, 4)) {
      WCastleQSide = false;
      WCastleKSide = false;
    } else if (move->source.value() == getPos(7, 4)) {
      BCastleQSide = false;
      BCastleKSide = false;
    }

    if (board[iSrc][jSrc] == 'k') {
      iwhiteKing = iDest;
      jwhiteKing = jDest;
    } else if (board[iSrc][jSrc] == 'K') {
      iblackKing = iDest;
      jblackKing = jDest;
    }
    
    board[iDest][jDest] = board[iSrc][jSrc];
    board[iSrc][jSrc] = '-';

    if (!lastMove) {
      free(lastMove);
    }

    lastMove = move;

}

Move* Bot::calculateNextMove() {
  /* Play move for the side the engine is playing (Hint: getEngineSide())
   * Make sure to record your move in custom structures before returning.
   *
   * Return move that you are willing to submit
   * Move is to be constructed via one of the factory methods declared in Move.h */

  // Piece piece = KING;
  vector<Move*> moves;
  PlaySide playSide = getEngineSide(); //
  int i, j;

  if (playSide == WHITE) {
    i = iwhiteKing;
    j = jwhiteKing;
  } else if (playSide == BLACK) {
    i = iblackKing;
    j = jblackKing;
  }

  // forcing castle
  if (canCastleKSide(playSide) == true && playSide == WHITE) {
    moves.push_back(Move::moveTo(getPos(0, 4), getPos(0, 6)));
  } else if (canCastleQSide(playSide) == true && playSide == WHITE) {
    moves.push_back(Move::moveTo(getPos(0, 4), getPos(0, 2)));
  } else if (canCastleKSide(playSide) == true && playSide == BLACK) {
    moves.push_back(Move::moveTo(getPos(7, 4), getPos(7, 6)));
  } else if (canCastleQSide(playSide) == true && playSide == BLACK) {
    moves.push_back(Move::moveTo(getPos(7, 4), getPos(7, 2)));
  } else if (isSquareAttacked(playSide, getPos(i, j)).size() != 0) {
    moves = getKingMoves(getPos(i, j));
  } else {
    moves = getAllMoves(playSide);
  }
  
  Move* move;
  srand(time(NULL));
  

  if (moves.size() == 0) {
    return Move::resign();
  }

  while (true) {
    
    unsigned int random = rand();
    random = random % moves.size();

    if (moves.size() == 0) {
    return Move::resign();
  }

    move = Move::moveTo(moves[random]->source.value(), moves[random]->destination.value());
    recordMove(move, playSide);

    if (isSquareAttacked(playSide, getPos(i, j)).size() != 0) {
      Move* move2 = Move::moveTo(moves[random]->destination.value(), moves[random]->source.value());
      recordMove(move2, playSide);
      moves.erase(moves.begin() + random);
    } else {
      break;
    }
  }

  deallocateVector(moves);

  return move;
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }
