---
title: "PAF Day 2: A Foray Into Game Design"
author: yberreby
category: devlog
tags: [assembly, setup]
layout: post
date: 2020-06-22 20:00 +0200
---

Here be dr..., uh, I mean lots of learning.

## On Time Dilation, AKA "The Weekend"

This was technically day 2 of the PAF, but we had a secret weapon...
the weekend.

We used it to get (relatively) up to speed on GBZ80 assembly
language, along with Team Rocket, and shared knowledge on the quirks of this
platform.

We all made progress (at varying speeds) on the Pong tutorial we had been using
as a base following @JeanMarieMineau's suggestion.

We discussed things like the infamous [Sprite RAM
bug](https://gbdev.io/pandocs/#sprite-ram-bug), the graphical limitations of the
PPU (which can handle a maximum of 10 sprites per line, 40 sprites total
on-screen).

I (@yberreby) also found [another introduction to GB
programming](https://eldred.fr/gb-asm-tutorial/introduction.html). I
followed it until part III.4 (the rest hasn't been written yet) and shared it
with the others.


Then, on Monday proper, after we felt that most of us had a decent understanding
of the unwelcoming environment we would be making our way through, we got 
talking about... _drum roll_ game design, and organization!


## Designing mechanics, or "making a game fun"

We decided to take time to think about the kind of game we would make. We want
to do something new, not just a clone of The Binding of Isaac - in part because
several of us haven't played it. We didn't have a very specific plan, so we
brainstormed in a delightfully erratic way for a number of hours about the
mechanics we wanted to implement.

We thought it would be cool to have **very fast / dynamic combat mechanics** rather
than just shooting ; we thought of adding a dashing and/or grappling hook
mechanism. There should be some kind of cooldown, or other limitation, to
prevent dashing from being overpowered.

The question of how to handle the **direction in which the characters' weapons are
pointing VS the direction of movement** came up. We might have a mechanism for
locking the direction of view, but we have to carefully consider how to
implement it given the limited number of buttons we have at our disposal.

We'd also like to have **lore** peppered through the game. We're still in the
process of defining precisely what it will be about, but having the player find
bits of backstory here and there should not be terribly difficult to implement,
while providing a lot of added value.

**Map generation and storage** was a major topic of discussion between @ChatPion
and myself (@yberreby). Since we're making something that looks like a roguelike
if you squint, we want procedural map generation. But not _everything_ will be
random ; we'll need to design portions of maps, and we won't do so directly in
binary.

So we thought of using 3 formats:
* A human-editable, JSON-based one (@ChatPion suggested [Ogmo Editor
  3](https://ogmo-editor-3.github.io/) for that),
* A compact binary format for storage in the ROM, using as few bits as possible
  for each piece of information we need to store,
* An unpacked one to use within game code, which would take up more space but be
  easier to use from the calling code's perspective (dealing with things like
  alignment unpacking 3-bit tile data is... unwieldy, to say the least).


We also started using CodiMD to create collaborative documents on game
design. They are editable in real-time and allow us to iterate on design
decisions with less friction. Shoutout to @ErwannE :)
