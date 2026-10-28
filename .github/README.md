<h1 align="center">
  CS100433<br>
  Computer Graphics
  <br>
  <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/palette/macchiato.png" width="600px"/>
  <br>
</h1>

<div align="center">
  <p></p>
  <div align="center">
  </div>
  <br>
</div>

<h2 align="center">
    Assignment 2
</h2>

## About the project

Here you will find the questions and answers to my second assignment of Computer Graphics class, with mathematical-code proof of my answer. And a small OpenGL project in last exercise.
<br>
The Source directory is separated in 8 parts:
```bash
.
├── Source
│   ├── Ex1
│   ├── Ex2
│   ├── Ex3
│   ├── Ex4
│   ├── Ex5
│   ├── Ex6
│   ├── Ex7
│   └── Lib
```
Each part corresponds to one exercise, and the `Lib` is my C++ library.

## How to build

This repository uses C++26 reflections. First, Install dependencies and build automatically using [**nix**](https://nixos.org/).
```bash
nix build
```

Or manually install dependencies (or use `nix develop`) and build with [**gnumake**](https://www.gnu.org/software/make/).
```bash
make
```
