---
title: "Integrating Piney's Foes"
author: AdameBf
category: monsters
tags: [enemies]
layout: post
date: 2020-07-01 14:00:00 +0200
---
AI development has reached a point where it could not continue without being integrated with the rest of the game.
And since integration was proclaimed main goal of the day by @yberreby, here goes!

## AI progress

In my previous post, I mentioned that *projectiles* would be the next feature I would be dealing with.
Discussions happened, and instead, the focus was put on *AI walking*.

How is it progressing? Well, it can move, in a direction arbitrarily defined to LEFT upon spawning!
Several improvements are lacking: there are no walking animations at the moment, but most importantly,
there hardly are any restrictions to AI movement, because collisions with either the terrain or the player
are not yet taken into account.

## AI integration

The above can only be properly handled if Piney and his foes toy around on the same playfield.
Integration is therefore a prerequisite at this point, and everything works so far!

<img src="/gboi-kirby/assets/img/enemies/ai_integration.gif" width="400"  />

## What's next?

Collisions should be properly handled by AI units. Code for player collisions could be reused,
but decision-making needs to be implemented as well (while a player attemping to walk against a wall
will just stay immobile until the player takes another action, AI should make a decision on its own).
