# Project Lux

<!-- 
Comment: You can create the "Table of Contents", by copying the content of the README.md to this page "https://ecotrust-canada.github.io/markdown-toc/"
-->
- [Project Lux](#project-lux)
  * [Description](#description)
  * [Guidelines and Best Practices](#guidelines-and-best-practices)
    + [Style guidelines for Unreal Engine 5](#style-guidelines-for-unreal-engine-5)
      - [Additional Asset Name Modifiers](#additional-asset-name-modifiers)
    + [Coding guidelines](#coding-guidelines)
    + [Git and GitHub guidelines](#git-and-github-guidelines)
      - [Commit messages](#commit-messages)
      - [Pull requests](#pull-requests)

## Description
Project Lux is a 2.5D Metroidvania game and is developed with the help of Unreal Engine version *5.0*.

## Guidelines and Best Practices
> **Note**:  
> None of the upcoming sets of rules and 'best practices' are final and are open for suggestions or own experiences. Feel free to create Issues regarding this topic. Furthermore we try as much as possible to maintain a clean and maintainable project, but we're still an unexperienced team and have a lot to learn.

### Style guidelines for Unreal Engine 5
We follow the *Gamemakin LLC* style guidelines, which can be found [here](https://github.com/Allar/ue5-style-guide). To simplify compliance with the rules the [Linter v2](https://www.unrealengine.com/marketplace/en-US/product/linter-v2?sessionInvalidated=true) plugin is available for UE4, but unfortunately not yet for version *UE5*. As soon as this is the case, we'll make use of it in our project.

#### Additional Asset Name Modifiers
In addition to the previously mentioned guidelines, we make use of the following project-specific asset name modifiers:

| Asset Type              | Prefix     | Suffix     | Notes                            |
| ----------------------- | ---------- | ---------- | -------------------------------- |
| Gameplay Ability        | GA_        |            |                                  |
| Gameplay Cue            | GC_        |            |                                  |
| Gameplay Effect         | GE_        |            |                                  |

### Coding guidelines
For our codebase we follow the coding standard provided by the developers of the Unreal Engine, which can be found [here](https://docs.unrealengine.com/5.0/en-US/epic-cplusplus-coding-standardblueprint-debugging-in-unreal-engine/).

### Git and GitHub guidelines
#### Commit messages
Our commit messages should follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) style, allowing a more structured commit history. In addition, we have established other rules that fit our project well:
- The commit message `<type>` should be followed by the `[optional scope]`, which refers to the top-level directory and subdirectory (separated by a hyphen `-`)  related to the change, e.g. `feat(maps):` or `fix(source-core):`. In case of general or unspecified changes the `[optional scope]` can be omitted.
- Under the commit message a `-file`, `-class` or `-asset` is followed by the related name, which is involved in the commit, e.g.
```
feat(source-core): Increased max velocity when walking

-class: ProjectLuxCharacter
```

#### Pull requests
With the development of a new feature (on a distinct feature-branch) a *pull request* should be created, as soon as the first commit is done. Before this is not possible, because GitHub only let's you create pull requests, when changes to the project appear. 

Doing so gives us the opportunity to offer suggestions for changes, review, point out errors and misunderstandings, etc. early and collects everything on a single page.

The pull-request should be created/set as "*Draft*" and "*Draft:*" added at the start of the title, until the work on the feature is over.
