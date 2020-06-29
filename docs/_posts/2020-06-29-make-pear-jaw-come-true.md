---
title: "Make Pear Jaw come true"
author: Adame
category: monsters
tags: [monsters, test]
layout: post
date: 2020-06-29 20:00:00 +0200
---

Following up on Ametheek's introduction to the wonderful (well, no longer) world of Piney and his foes, here is how far I got with implementing enemy monsters.

## Enemy monsters: progress

Currently, here are the features enemy monsters are to have in the game as of now, after a long phase of figuring out things all along:
- It is now possible to display 16×16 foes on screen, such as *Jaw Pear*, and wherever you want! Yey!
- Health bar management is implemented: an enemy can lose or gain HP, and properly dies when brought down to 0 HP.
- Enemies deal damage directly based on their damage stat.
- How often an enemy attacks depends on their "attack speed" stat (implemented as the number of frames between attacks in-game).
- Attack types are ready to be handled (they just need to be implemented). Two types of attacks are intended: melee attacks and global range projectiles.
All of the above can be customized when creating a new enemy. The last item in this list needs to be completed: for now, the only type of attack available is a self-damaging attack (it ensures that the other stats mentioned in the list work as intended!).

## What needs to be done

- Handle melee attacks. This will likely involve: detecting whether the player is in immediate vicinty of the monster, handling the attack animation and its speed, so as to give the player just the right time to react, effectively inflicting damage to the player.
- Handle projectiles. This will likely involve: deciding in which direction to throw them (should depend on the player's position), launching them, transmitting the damage value to the projectile, handling the projectile travel speed, ensuring collisions between the projectile and the player are detected, ensuring that the player incurs damage in the event of a collision and absorbs the projectile.
- Handle projectiles. Yes, again. But this time, I am talking about handling projectiles thrown *by* the player, not at them. This should be fairly similar to the above, except of course that the projectile should be absorbed by any enemy rather than by the player.
- Handle AI walking: especially, the decision-making when colliding with (a) obstacles on the landscape or (b) the player. A melee enemy colliding with the player will most likely want to stay in melee range, for example.

## Next goal

I am considering the handling of projectiles next, at this point.
