---
title: "On Map Generation (part 1)"
author: ChatPion
category: mapgen
tags: [mapgen]
layout: post
date: 2020-06-24 09:32:09 +0200
---

Roguelikes' replayability comes from one important factor: procedurally generated maps. In _The Binding of Isaac_, maps are made up of connected rooms and only the layout is randomly generated − the rooms being sampled from a set of handmade rooms.

In our game, we follow the same map structure, but I won't cover floor generation in this post (hence the "part 1" in the title). Instead, I'm going to talk about room generation.

## What do we need?

There are two kinds of room: special rooms (spawn, treasure room, boss room...) and common rooms. 

Special rooms are specials and are not covered in this post, but they will get a special treatment, that's how special they are!

Common rooms are split into two categories: small rooms (16×16) and big rooms (24×24). 

There are several types of tiles :

- normal ground
- solid rocks
- breakable blocks
- holes
- spikes
- chests
- signs

We cannot afford to manually design these rooms, it would be too much time consuming, that's why we want to procedurally generate them.

## Our algorithm

After brainstorming with [@RoussyE](https://github.com/RoussyE), here's what we came up with:

Rooms are split into handcrafted 8×8 chunks. For the small rooms, we only need to design one "corner chunk", since the others can be obtained by flipping it along the X or Y-axis or by rotating it. Let's arbitrarily choose the top-left chunk.

Then, what about doors? We do not want to accidentally block them, so we have to mark our chunks as "top-door-able" and "left-door-able" (we only create top-left chunks!). We will use 2 extra bits for that.

Then, what about big rooms? Big rooms are made up of four "corner chunks", four "door chunks" and one "center chunk". Does that mean that we have to create _three times_ more rooms? No, that would be too much time and memory consuming! Let's be smarter, and _reduce, reuse, recycle_. 

Instead of manually creating "door chunks", we could instead combine two halves of a corner chunk with the correct flags : 

```
top-door-able     top-door-able
   chunk        chunk (X-flipped) 
  XXXXAAAA          BBBBXXXX          AAAABBBB
  XXXXAAAA          BBBBXXXX          AAAABBBB
  XXXXAAAA          BBBBXXXX          AAAABBBB
  XXXXAAAA    +     BBBBXXXX     =    AAAABBBB
  XXXXAAAA          BBBBXXXX          AAAABBBB
  XXXXAAAA          BBBBXXXX          AAAABBBB
  XXXXAAAA          BBBBXXXX          AAAABBBB
  XXXXAAAA          BBBBXXXX          AAAABBBB
```

Finally, center chunks can reuse corner chunks. To avoid placing chunks that naturally require surrounding walls at the center of the room, we will flag these chunks as "pure corner chunks" and only use non-flagged chunks.

### Notes

* We might need to perform some blending operations between chunks to have an homogeneous room.
* After testing on paper, we should introduce an overall "clutter" level per chunk to prevent the algorithm from creating over-bloated rooms.

## How much size is required?

Let's do some quick maths:

- if we wanted to be super optimized, we could use 3 bits to encode one tile, but to save ourselves from suffering, we will encode one tile with 4 bits. Thus, one tile needs half a byte.
- a chunk is an 8×8 block
- we need an additional byte to store flags (corner, northern door, eastern door)

Therefore, one chunk occupies 33 bytes, thus we could store about 31 chunks in 1 kB!


## Results (on paper)

[@RoussyE](https://github.com/RoussyE) and I drew 8×8 chunks on paper and randomly assembled them to create rooms. So far, some rooms look _okayish_, but most of them are terribly over-bloated − that's when we thought of introducing clutter levels.

How to read: 

- black boxes = hole
- triangle = rock
- "grass" = spikes
- cross = enemy spawner

<img src="/gboi-kirby/assets/img/small_map1.jpg" width="450"  />
<img src="/gboi-kirby/assets/img/small_map2.jpg" width="450"  />
<img src="/gboi-kirby/assets/img/small_map3.jpg" width="450"  />
<img src="/gboi-kirby/assets/img/big_map1.jpg" width="600"  />
