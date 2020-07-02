---
title: "PAF... It's The End!"
author: yberreby
category: devlog
tags: []
layout: post
date: 2020-07-02 19:00:00 +0200
---

It's over. Today marks the last day of our end-of-year project for Télécom
Paris... and what a day (and, for part of our team, *night*) it was!

Time to ship in and integrate ultimate features (preferrably non-game-breaking
ones ;] ) for a little demo in front of other people. We can definitely be proud
of our playable game (albeit somewhat buggy, but pssh...), which came to life in
less than two weeks!

We made a (final? time will tell...) release, so you can [try it
out](https://github.com/PainsPerdus/gboi-kirby/releases/tag/1.0) by downloading
our ROM and running it under a Game Boy emulator (we use
[SameBoy](https://sameboy.github.io/), but here's a little demo (with sound!):

<video controls height="400" >
  <source src="/gboi-kirby/assets/1.0-demo.mp4" type="video/mp4">
</video>


## Successful ~~last-minute~~ integrations

* [@PaulVez][1]'s vertical chainsaw animation!
* [@AdameBf][6]'s enemy logic - thanks to him, demonic pears now roam the chunks
  in search of the player, driven by ever-present hunger for some tasty
  pineapple...
* ...but fear not, our dear Piney now has the means to defend himself, because
  his chainsaw [**finally
  works**](https://github.com/PainsPerdus/gboi-kirby/pull/73/files) and does
  damage to his oppponents!
* [@RoussyE][3]'s sound effects had been lying dormant so far, but now some of
  them are actually used (for dashing, the chainsaw attack, falling in holes,,
  taking damage)!
* [@yberreby][5] created a minimalistic UI using [@Ametheek][4]'s sprites for
  the health and stamina bar, and [@ChatPion][2] integrated it with the game
  logic


## Failur... uh, I mean, setbacks

<img height="400" src="/gboi-kirby/assets/img/titlescreen.png" />

The **title screen** (see above) oozes cuteness and psychopathy, thanks to
[@Ametheek][4] artistic touch, and was supposed to double as a way to initialize
our random number generator by counting the frames before the player presses
`Start`. Unfortunately, that plan turned sour when every single one of our
attempts at writing the said title screen corrupted the game's memory.

The **blaster sound** was awesome, especially considering the limitations of the
Game Boy sound system: 

<audio
controls
src="/gboi-kirby/assets/blaster.mp3">
Your browser does not support the
<code>audio</code> element.
</audio>

but we could not implement projectiles in time to make use of it :'(



## On sound design

>  Thank to the sound features of the Gameboy, every sound is implemented as a single
C function. These functions contain inline ASM, assembled and inserted as-is in
the generated machine code. They modify the sound controller register of the
Gameboy (addresses FF10 to FF26) and use the sound input 1 (tone and sweep) and
4 (white noise).  Because of the many weird behaviors of the Game Boy, the only
way to create these sounds was: testing... and testing...  and mind fucking...
and testing.  So this was pretty horrible but through this, I could create many
funny sounds, like the Star Wars blaster sound, which is not used in-game but is
still in the code ;P

- [@RoussyE][3]

## On AI

> The last few hours since my last post were rich in features for _Pear
Jaw_ (or _Jaw Pear_, I'm feeling tolerant today)! This little thing can now
move, and it automatically decides to walk towards Piney, and will _sort of_
take into account terrain obstacles while doing so (this just needs further
fine-tuning for it to be consistent!).

> And in fact, I should be talking about multiple _Jaw Pears_ now.
[@ChatPion][2] implemented proper internal management for multiple foes, and
that includes handling enemy spawning for each room in the map! And while these
foes are mostly harmless, you are locked with them in your room until you
take them out with your chainsaw. And really, don't get fooled by how
harmless _Jaw Pear_ seems to be for now. There is only so much this soft
fruit of destruction can learn about close combat with some practice!

- [@AdameBf][6]


## Last words... till we meet again!

This project was not easy. We all had to sacrifice something to make it word, be
it committment to good coding practices, sleep, sanity, or any of the many other
things we lost along the way; but we also *gained* a great deal, and I'm not
only talking about our freshly-minted knowledge of the Game Boy hardware
architecture. I think I speak for the entire Team Kirby when I say that this project
was exhilarating; it pushed us to our limits, and showed us just how much we
could achieve, together, with focus and dedication. So to all members of Team
Kirby, of Team Rocket (our friends and competitors - check our [their
game][7] if you like low-level stuff, it should satisfy anyone's craving for
GBZ80 ASM *and* clever hacks), and to our supervising teacher
[@Aunsiels][8]... so long, and thank you for all the ~~fish~~ glitches :)

[1]: https://github.com/PaulVez
[2]: https://github.com/ChatPion
[3]: https://github.com/RoussyE
[4]: https://github.com/Ametheek
[5]: https://github.com/yberreby
[6]: https://github.com/AdameBf
[7]: https://github.com/PainsPerdus/gboi-rocket
[8]: https://github.com/Aunsiels
