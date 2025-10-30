from games import *


class GameOfNim(Game):
    """Play Game of Nim with first player 'MAX'.
    A state has the player to move, a cached utility, a list of moves in
    the form of a list of (x, y) positions, and a board, in the form of
    a list with number of objects in each row."""

    def __init__(self, board=[3, 1]):
        moves = []
        for row_idx in range(len(board)):
            for num_objects in range(1, board[row_idx] + 1):
                moves.append((row_idx, num_objects))

        self.initial = GameState(to_move="MAX", utility=0, board=board, moves=moves)

    def actions(self, state):
        """Legal moves are at least one object, all from the same row."""
        return state.moves

    def result(self, state, move):
        row_idx, num_to_remove = move

        new_board = state.board.copy()
        new_board[row_idx] -= num_to_remove

        new_moves = []
        for r in range(len(new_board)):
            for n in range(1, new_board[r] + 1):
                new_moves.append((r, n))

        next_player = "MIN" if state.to_move == "MAX" else "MAX"

        return GameState(
            to_move=next_player,
            utility=self.compute_utility(new_board, move, state.to_move),
            board=new_board,
            moves=new_moves,
        )

    def utility(self, state, player):
        """Return the value to player; 1 for win, -1 for loss, 0 otherwise."""
        return state.utility if player == "MAX" else -state.utility

    def terminal_test(self, state):
        """A state is terminal if there are no objects left"""
        return all(count == 0 for count in state.board)

    def compute_utility(self, board, move, prev_player):
        if all(count == 0 for count in board):
            return 1 if prev_player == "MIN" else -1
        else:
            return 0

    def display(self, state):
        board = state.board
        print("board:", board)


if __name__ == "__main__":
    nim = GameOfNim(board=[0, 5, 3, 1])
    print(nim.initial.board)
    print(nim.initial.moves)
    print(nim.result(nim.initial, (1, 3)))

    utility = nim.play_game(alpha_beta_player, query_player)
    if utility < 0:
        print("MIN won the game")
    else:
        print("MAX won the game")
