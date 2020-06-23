---
title: "Setting up Jekyll with Github Pages!"
author: ChatPion
category: setup
tags: [jekyll, setup]
layout: post
date: 2020-06-20 12:11:23 +0200
---

Since I had trouble setting up this devlog, I'm leaving here some notes for my future self.

## Project structure

Note: we chose to put the website in the `master` branch in the `docs` folder at the project root.

Steps:

* create a `docs` folder and push it to Github
* initialize Jekyll in `docs` (`bundle exec jekyll new .`)  
* in Github, select the wanted theme
* replace `index.markdown` with the following `index.html`

```html
---
layout: default
title: Blog
---

<ul>
  {% for post in site.posts %}
    <li>
       <h2><a href="{{ site.baseurl }}{{ post.url }}">{{ post.title }}</a></h2>
       {{ post.excerpt }}
    </li>
  {% endfor %}
</ul>
```

* in `_config.yml`, set `baseurl` and `url` to the required url (for this website : `/gboi-kirby`)

This should be enough to setup a basic blog.

Note: the jekyll-asciidoc plugin does not work out of the box with github pages.
