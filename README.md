# Monty Hall Problem Simulation with Multithreading

## Overview

This project is a simulation of the **Monty Hall problem**, designed to handle multiple clients and servers concurrently using **multithreading**. The goal is to demonstrate the famous probability puzzle while showcasing how threads can be used to manage several simultaneous interactions.

## Purpose

The main purpose of this project is to simulate the Monty Hall problem in a client-server architecture. Each client can participate in the Monty Hall game, and the server responds by handling multiple clients concurrently using threads.

The Monty Hall problem is a classic probability puzzle that challenges a player's intuition. This project helps to visualize the problem's results across multiple iterations with different clients playing the game at the same time.

## Functionality

- **Simulates the Monty Hall problem**: A game host offers a player three doors, one of which hides a prize. The player selects one door, the host reveals a losing door, and the player can either switch or stay with their initial choice. The simulation tracks the outcome over many iterations.
- **Multithreaded client-server model**: Each client runs a separate game instance, and the server manages multiple clients simultaneously using threads.

## How it Works

- The server listens for incoming connections from multiple clients.
- Each client connects to the server and participates in the Monty Hall game.
- Once connected, the client selects a door, and the server reveals one of the remaining doors that does not contain the prize.
- The client can choose to switch or stay with their original choice.
- The server responds with the outcome (win or lose), and the game can be repeated as needed.
