# Path Finding

---

![alt-text](https://github.com/alessandro90/dijkstra_Cpp/blob/master/dijkstra.gif)

---

This is a basic implementation of the [Dijkstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) algorithm to find the shortest path between two points on a plane.

The code uses some features of C++20 and is clearly overengineered. I wrote it just to have fun.

There are 3 files in the `text_files` folder:

-   _example.txt_ Is an example of level. `*` is an allowed point (the algorithm can visit it), `X` is an obstacle. `A` is the starting point and `B` is the destination point. The file can contain only these characters. Also, the level doesn't have to be rectangular (some rows can be longer/shorter than others).

-   _config.txt_ A very basic configuration file:

    -   _edgeWidth_ Width (pixels) of each cell;
    -   _edgeHeight_ Height (pixels) of each cell;
    -   _maxFrameRate_ Each iteration will take **at least** this value in milliseconds;
    -   _graphPath_ File path to a level relative to the executable.

-   _config_i.txt_ A very basic configuration file for the interactive mode:
    -   _edgeWidth_ Width (pixels) of each cell;
    -   _edgeHeight_ Height (pixels) of each cell;
    -   _maxFrameRate_ Each iteration will take **at least** this value in milliseconds;
    -   _rows_ Number of rows of the window;
    -   _cols_ Number of columns of the window.

## Run

Simply run `./dijkstra` to load a file as `example.txt`.

Run `./dijkstra -i` to run in interactive mode:

-   The windows starts empty and the configuration in `config_i.txt` is used.

## Commands

-   `Enter` Start the algorithm;
-   `Escape` Restart with a fresh window;
-   Before pressing `Enter` the cells can be edited:
    -   Start and end points can be dragged;
    -   Obstacles can be added(removed) by left(right)-clicking with the mouse

## Compilation

`mkdir build && cd build && cmake .. && make`

Note that you will need [SFML](https://www.sfml-dev.org/) and a C++20 compiler.
