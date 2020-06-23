---
title: "PAF Day 1: Hello world!"
author: ChatPion
category: devlog
tags: [assembly, tooling]
layout: post
date: 2020-06-19 20:55:00 +0200
---

I hereby declare that this devlog is open! What is this devlog about? We, team
Kirby, are going to make a _The Binding of Isaac_-inspired game for the original
Game Boy and we will talk about our ideas, plans, difficulties and progress
here.

## Today

Here's a quick recap of what happened:

* Install party
* Setting up our environment on GitHub
* Crash course in gameboy assembly language
* Practice

### Install party

Basically, we ensured everybody is able to assemble assembly programs and run it on a gameboy emulator (vba-m or BGB)

* Setup the Windows Subsystem for Linux (to make everyone use the same assembly toolchain)
* Setup the assembly toolchain
* Setup a gameboy emulator (vba-m, bgb)

### GitHub

We properly set the git repositories up: 

* choose our workflow (one protected master branch + pull requests + code reviews)
* manage permissions
* setup the devlog using Jekyll

We intend to use as many available features as we can (pull requests, code reviews, releases, github pages, etc.)

### Gameboy assembly language

Basically, [we used this repository](https://github.com/PainsPerdus/gb_training)
made by our dear Jean-Marie from Team Rocket.

Even though this language is not complex per se, we still struggled because of
some subtleties, such as : 

* not all combinations of instructions and operands are available, like `ld (nn),nn`
* spaces _are_ important: labels are written `label:` and _not_ `labels :`


## What kind of game are we going to make?

Whilst the other team plans to make an accurate reproduction of _The Binding of
Isaac_ (and adapt the twin stick shooter gameplay to the gameboy), we intend to
keep the _spirit_ of the game (a roguelike + some features) and make our own
game.

## Coming up next

During the week-end:

* complete tutorials (code a _Pong_)

Monday, we planned to do the following:

* give a git crash course (redo the basics if needed and show our planned git workflow)
* define the calling conventions
* define the gameplay

## Random thoughts

* a crash course in debugging using vba-m or bgb would be nice
* we should consider pair programming
