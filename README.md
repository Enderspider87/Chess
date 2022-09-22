# Chess

This is a command line chess program. It supports every type of move in chess, including promoting pawns, en passant, and castling. However, it is not a chess engine, meaning that it does not evaluate moves to determine if they are good or bad. In this document, I will explain how I went about building this program, as well as some of the obstacles I faced and how I overcame them.

The basic order in which I approached this project was the following:
- Get convertChess and reverseConvert working. These functions convert to and from chess notation (eg. e2) and rows and columns. 
- Build the board and create the pieces using dynamic allocation
- Implement a move functionality
- Define the basic legal moves for each piece
- Implement the inCheck function, which determines if a king is in check by looking at the legal moves of the opposite color pieces.
- Create a "legal board", which displays the legal moves that a piece has once selected.
- Set up checkmate and stalemate, which happen when one side has no legal moves. If a player is in check, the opposite player wins. Otherwise, stalemate.
- Allow pawns to promote when they reach the end.
- Implement a move type system to accommodate en passant and castling.
- Evaluate if en passant is legal.
- Evaluate if castling is legal.
- Set up a function to evaluate the move type and perform the appropriate move.

Obstacles:

- Displaying Legal Moves

For this, I had to set up a "legal board", which has Legal objects. The way I set this up is that after the user selected the piece they wanted to move, I created a vector to store all the squares that piece could move to. Before I set up En Passant and Castling, I would create a new Legal object (*) with dynamic allocation, or I would add the move to a legal capture moves vector if the square had a piece with the opposite color. Once I added en passant and castling, I changed the constructor of Legal to accept a parameter, which is the move type, represented by an int. The getPiece function, as defined in the Legal class, checks the move type and displays the appropriate symbol on the console (* for normal move, E for en passant, or C for castling).

- En Passant

The toughest part of coding en passant is that whether or not the move is legal depends on the other pawn's move history. As a result, I had to set up some type of data structure to store a piece's move history. My initial thought was to use a stack for this, but I discovered that using a vector instead was easier because it would be easier to access the individual elements, a necessity for evaluating en passant. My final solution was a vector of tuples (string, string, and int). The first string stores the initial position, the second string the final position, and the int stores the move number where the move took place. For en passant, the pawn to be captured (named "oppPawn" in my code) has to have made exactly one move, which is two ranks (rows) forward, and that move must have been the move right before en passant. If any of those conditions are violated, en passant is not legal.

Of these three tuple elements, the moveCount was the toughest to implement. The solution I settled on here is having move counts for each player, and when it came time to check if the pawn just moved, I added the two counts together to get the total move count. If this count was equal to the move count 

Note: I am aware that there is an edge case where the king and opposite color rook could be on either side of the two pawns, and in this case, en passant should be illegal because it would put the king in check. However, my code does not yet account for this. I intend to fix this problem at a later date.

- Castling

I used the same vector of tuples as I did for en passant, except all that matters is that the vectors for both the rook and king are empty, as they must not have moved. I then checked for obstructions, and if the king was currently in check, and finally if moving one and two squares over were valid moves (would they put the king in check?). If all of these conditions are satisfied, castling is legal.

Improvements for the future:

- Better method of identifying the king's check status

The way my program checks the king's check status (it's hard to avoid the pun) is by looking at the valid moves of all the opposing pieces. An earlier program also checked the opposite king's check status while verifying if that attacking move was valid, which was catastrophic because that would sometimes enter an infinite loop and crash my program. To fix this, I split the process of verifying a move's legality into two functions: a polymorphic function that checks if a piece can move to a specified square, and a non-polymorphic function that identifies if the move in question would put the king in check. When I check if an opposing piece can move onto the king's square, I only run that through the first function, since the current player's king takes precedent over the opposite king's status. While this process is more efficient than the first process I had, it is rather ugly, and I believe that there is a better way.

A more efficient way of doing this might be to set up an array of squares that the king cannot move to, as well as to keep track of obstructing pieces. This way, these obstructing pieces can be marked as being only able to move to another obstructing square.

- En Passant Problem

As I mentioned under en passant, my program does not handle the edge case of a rook being able to attack the king after en passant correctly. 

My idea for this is to add a parameter to the function that checks if the king is under attack that takes in moveType as an int, and if it is 2, which is the value for an en passant move, temporarily remove the opposing pawn and then put it back after the king's check status has been checked. With the opposing pawn removed, the normal function to see if a move puts the king in check suffices.
