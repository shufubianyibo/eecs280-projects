# EECS 280 — Programming and Data Structures

C++ coursework: image processing, Euchre, text classification, linked lists, text buffers, and binary search trees.

Coursework portfolio of **Han Sun ([shufubianyibo](https://github.com/shufubianyibo))**. This repository collects source snapshots from local projects so they can be browsed directly from my personal GitHub profile.

## Main projects

### [Text buffer and linked-list implementation — Project 5](p5/)

A C++ text-editing backend with insertion, deletion, and cursor movement across lines. It maintains cursor position together with row, column, and absolute index invariants. A separate templated doubly linked list implements node ownership and bidirectional iteration, with public and student-written tests.

Explore [`TextBuffer.cpp`](p5/TextBuffer.cpp), [`List.hpp`](p5/List.hpp), and [`List_tests.cpp`](p5/List_tests.cpp). In this snapshot the text buffer selects `std::list<char>`; the custom list is retained as its own implementation and can be selected using the aliases documented in the source.

### [Euchre card-game engine — Project 3](p3/)

A command-line card game organized into card, pack, player, and game components. The implementation covers card comparisons, trump-dependent ordering, dealing, player decisions, trick resolution, and scoring. Tests are separated by component, and game fixtures provide expected output for complete runs.

Start with [`euchre.cpp`](p3/euchre.cpp), [`Player.cpp`](p3/Player.cpp), and the card/pack tests. The project emphasizes coordinating several abstractions around a stateful game.

### [Content-aware image resizing — Project 2](p2-image-processing/)

A matrix- and image-processing pipeline that resizes PPM images using seam carving. Pixel energy is converted into a cumulative cost matrix, a low-cost vertical seam is traced through the image, and seam removal reduces image width while following image content.

Read [`processing.cpp`](p2-image-processing/processing.cpp), [`Matrix.cpp`](p2-image-processing/Matrix.cpp), and [`Image.cpp`](p2-image-processing/Image.cpp). Image fixtures and unit tests exercise the underlying operations.

### [Search trees and text classification — Projects 6 and 4](p6/)

[Project 6](p6/) implements a templated binary search tree and map, with iterator and ordering tests. [Project 4](p4/) builds a bag-of-words text classifier from CSV posts, using label priors and word likelihoods in log space. These projects apply generic data structures and probability-based decisions to different tasks.

## All projects

| Project | Source files |
| --- | ---: |
| [p2-image-processing](p2-image-processing/) | 18 |
| [p4](p4/) | 2 |
| [p3](p3/) | 14 |
| [p5](p5/) | 11 |
| [p6](p6/) | 9 |
| [starter-files](starter-files/) | 10 |
| [eecs280_pokemon_lab](eecs280_pokemon_lab/) | 9 |

## Running the projects

Each directory is independent. Read its README, build file, tests, and `PORTFOLIO.md` before running. Original course infrastructure, data, or toolchains may be required. This publication checks source packaging; it does not certify that every historical project builds on a current machine.

## Attribution

These are individual and collaborative course projects. Course starter code, supplied tests, and third-party dependencies are included where needed. Original credits and licenses are retained. Each `PORTFOLIO.md` links to the source repository and records its base commit and commit authors when available. Source-file counts include tests and scaffolding and are not a measure of individually authored work. Snapshots were collected on 2026-11-17; original working directories were left unchanged.
