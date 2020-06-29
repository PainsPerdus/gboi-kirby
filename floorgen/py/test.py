from random import randrange
from enum import Enum


BEGIN_CTX = 0
ADD_ROOM = 1
BOSS = 2
KEY = 3
END_CTX = 5
ITEM = 6


FLOOR_SIDE = 8  # MUST BE A POWER OF 2


class Room:
    def __init__(self, pos, boss=False):
        self.pos = pos
        self.boss = boss
        self.neigh = [None, None, None, None]

    def nb_neigh(self):
        return (self.neigh[0] != None) + (self.neigh[1] != None) + (self.neigh[2] != None) + (self.neigh[3] != None)


# nb_keys = randrange(0, 4)
# nb_items = randrange(1, 4)
nb_rooms = randrange(10, 20)

print(nb_rooms)

build_list = [ADD_ROOM for _ in range(nb_rooms-1)]
build_list.append(BOSS)
print(build_list)
build_list_ptr = 0

# nb_keys = randrange(0, 4)
# pour le moment on va se focaliser sur la construction

floor = [None for _ in range(FLOOR_SIDE * FLOOR_SIDE)]
built = [None for _ in range(nb_rooms)]

def print_floor():
    print("X" * FLOOR_SIDE)
    for x in range(FLOOR_SIDE):
        for y in range(FLOOR_SIDE):
            room = floor[coord(x, y)]
            if (room is None): 
                print(".", end="")
            elif (room.boss):
                print("B", end="")
            else:
                print(room.nb_neigh(), end="")
        print("")


error_flag = False

def coord(x, y):
    return FLOOR_SIDE * y + x


def is_valid(room):
    return room.nb_neigh() < 4

def get_available_room():
    global error_flag
    candidate_i = randrange(0, build_list_ptr)
    loop_size = 0
    while not is_valid(built[candidate_i]) and loop_size < build_list_ptr:
        candidate_i += 1
        if (candidate_i >= build_list_ptr):
            candidate_i = 0
        loop_size += 1

    if loop_size == build_list_ptr:
        error_flag = True
        return None

    return built[candidate_i]

UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

def inv_dir(delta):
    return [DOWN, UP, RIGHT, LEFT][delta]

def get_available_pos(room):
    global error_flag
    x = room.pos % FLOOR_SIDE  # modulo
    y = room.pos >> 3 # division
    available = []
    nb_available = 0
    if x > 0 and floor[coord(x - 1, y)] is None:
        available.append(LEFT)
        nb_available += 1
    if x < 7 and floor[coord(x + 1, y)] is None:
        available.append(RIGHT)
        nb_available += 1
    if y > 0 and floor[coord(x, y - 1)] is None:
        available.append(UP)
        nb_available += 1
    if y < 7 and floor[coord(x, y + 1)] is None:
        available.append(DOWN)
        nb_available += 1
    if nb_available == 0:
        error_flag = True
        return None

    return available[randrange(0, nb_available)]


def pop_room():
    global build_list_ptr
    build_list_ptr -= 1
    floor[built[build_list_ptr].pos] = None

def push_room(room):
    global build_list_ptr
    built[build_list_ptr] = room
    floor[room.pos] = room
    build_list_ptr += 1
    
root = Room(coord(FLOOR_SIDE >> 1, FLOOR_SIDE >> 1))
push_room(root)
print_floor()

def make_pos(pos, delta):
    if delta == UP:
        return pos - FLOOR_SIDE
    elif delta == DOWN:
        return pos + FLOOR_SIDE
    elif delta == LEFT:
        return pos - 1
    elif delta == RIGHT:
        return pos + 1



while build_list_ptr < nb_rooms:

    # First we build a room
    error_flag = False
    room = get_available_room()
    pos = room.pos
    if error_flag:
        pop_room()
        continue

    error_flag = False
    delta = get_available_pos(room)
    if error_flag:
        pop_room()
        continue
    new_pos = make_pos(pos, delta)
    new_room = Room(new_pos)
    room.neigh[delta] = new_room
    new_room.neigh[inv_dir(delta)] = room

    if build_list[build_list_ptr] == ADD_ROOM:
        for i in range(0, 4):
            if (0 <= (test_pos := make_pos(new_pos, i)) < FLOOR_SIDE * FLOOR_SIDE) and (test_room := floor[test_pos]) is not None:
                new_room.neigh[i] = test_room
                test_room.neigh[inv_dir(i)] = new_room

    elif build_list[build_list_ptr] == BOSS:
        new_room.boss = True

    push_room(new_room)
    print_floor()
