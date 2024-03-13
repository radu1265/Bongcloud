#ifndef BOT_H
#define BOT_H
#include <bits/stdc++.h>

#include <vector>
#include "Move.h"
#include "PlaySide.h"

#define SIZE 8

using namespace std;

class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */

   vector<vector <char>> board;
   //vector<Move*> myMoves;


   int movesWithoutCaptureOrPawnMove;

   bool WCastleQSide;
   bool WCastleKSide;
   bool BCastleQSide;
   bool BCastleKSide;

   int iwhiteKing;
   int jwhiteKing;

   int iblackKing;
   int jblackKing;

   Move* lastMove;


  /* Declare custom fields above */


  Bot();

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move* move, PlaySide sideToMove);

  
  /**
   * Calculates and return the bot's next move
   * @return your move
   */
  Move* calculateNextMove();

  static std::string getBotName();

  // implemented by me

  int getRow(string pos);
  int getCol(string pos);
  string getPos(int i, int j);
  bool isEnemyPiece(char piece, char enemy);
  bool isMyPiece(char piece, char myPiece);
  bool isFreeSquare(char square);
  vector<Move*> getPawnMoves(string source, PlaySide mySide);
  vector<Move*> getKnightMoves(string source);
  vector<Move*> getKingMoves(string source);
  vector<Move*> getRookMoves(string source);
  vector<Move*> getBishopMoves(string source);
  vector<Move*> getQueenMoves(string source);
  vector<Move*> getAllMoves(PlaySide sideToPlay);
  void deallocateVector(vector<Move*> &moves);
  void insertVector(vector<Move*> &dest, vector<Move*> src);
  vector<string> isSquareAttacked(PlaySide sideToPlay, string src);
  bool canCastleQSide(PlaySide playSide);
  bool canCastleKSide(PlaySide playSide);

};


#endif
