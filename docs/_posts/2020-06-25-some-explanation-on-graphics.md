---
title: "Some explanations on graphics"
author: Ametheek
category: graphics
tags: [graphics, test]
layout: post
date: 2020-06-25 09:32:09 +0200
---

Even though our project is based on _The Binding of Isaac_, we decided to create
our own visual identity, with an original main character.

?

## First steps

At first, when we started to talk about our project, we at Team Kirby wanted to add
new gameplay ideas, and we chose to draw inspiration from the _Doom_ games.
We wanted a badass character, ready to murder anyone in a whirlwind of blood
and violence, and we thought that making it look innocent would be in hilarious
contrast with its psychopathic behavior. So our goal was to make something both
cute and murderous.

## Meet 'Piney'

At first, we tried to make an 8x8 character. We wanted to reduce the number of
horizontal sprites to a minimum (given that only 10 can be displayed on a given
line), and we thought it would be hilarious to have an undersized main character
wielding huge weapons and wreaking havoc on enemies several times its size.

Alas, 8x8 was too small for us to manage getting both the **smile** (for the
'psychopathic' component) and the **ears/horns/hair** (for the 'cute' component)
right, so we chose to go with an 8x16 sprite.

After some trial and error, we created **Piney**:

<img src="/gboi-kirby/assets/img/latwalk1.gif" width="150"  />
<img src="/gboi-kirby/assets/img/downwalk1.gif" width="150"  />


Piney is a pineapple. Piney is cute. Piney will murder anyone without remorse.

### Piney's preferred way of killing: up close, bloody and personal

At that point, we had the cuteness down. But we needed to show Piney's, uh,
_peculiar_ tendencies in some way, and what better way to do that than through
the gameplay?
More specifically, through its weapon and favorite way of killing. And because
_Doom_ was a major inspiration, we easily agreed to give our little Piney an
oversized  chainsaw.

_NB: getting it to be oversized without wasting sprites is proving to
be a challenge._

At first, it was just supposed to be one of our mad pineapple's many weapons,
but as discussions proceeded, we agreed that it should have a special place and
become our character's iconic weapon.

Here is a preliminary version of it in action:

<img src="/gboi-kirby/assets/img/chainsaw1.gif" width="150" />


### What more?

For the time being, I'm focusing a lot on the chainsaw. It must be messy to
use, oversized, and I brainstormed a lot with
[@yberreby](https://github.com/yberreby) to know what to do with it.

It will be used in combination with another game mechanic that is yet to be
implemented: the **Dash**, a super-fast move that should be high-risk,
high-reward, and high-fun.

It's supposed to be a major part of the gameplay, so I need to offer Piney a
good dash animation in order for the player have a good gameplay experience.


## Maps and general aestethics

I'm working with [@Chatpion](https://github.com/ChatPion) and
[@RoussyE](https://github.com/RoussyE) to create the graphics they need for the
maps (pikes, walls, holes...).

Because we want to make a violent game (with blood and explosions!) we will need
lots of sprites/tiles (we haven't decided how to make it yet) to display all
that information on screen. Also, I decided to have very simple
map graphics at first, so as not to saturate the visual information so that the
player can manage to see what's going on despite the bloodshed.
