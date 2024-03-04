# Project Lux
<!-- 
Comment: You can create the "Table of Contents", by copying the content of the README.md to this page "https://ecotrust-canada.github.io/markdown-toc/"
-->
- [Project Lux](#project-lux)
  * [Description](#description)
  * [Project Management](#project-management)
  * [Guidelines and Best Practices](#guidelines-and-best-practices)
    + [Style Guidelines for Unreal Engine 5](#style-guidelines-for-unreal-engine-5)
      - [Additional Asset Name Modifiers](#additional-asset-name-modifiers)
    + [Coding Guidelines](#coding-guidelines)
      - [Additional Naming Conventions](#additional-naming-conventions)
    + [Git and GitHub Guidelines](#git-and-github-guidelines)
      - [Commit Messages](#commit-messages)
      - [Branching](#branching)
      - [Pull Requests](#pull-requests)

## Description
Project Lux is a 2.5D Metroidvania game and is developed with the help of *Unreal Engine 5*.

## Project Management
For collaboration and progress monitoring, a project management system was set up in _Notion_. The workspace can be reached via this link: [tinyalmonds.notion.site](https://tinyalmonds.notion.site/Project-Lux-9a1c528202e5495eaad2026c52145a74?pvs=4). You can get workspace editing/commenting rights from [Alex Nördemann](https://github.com/alexnoerdemann).

## Guidelines and Best Practices
> **Note**:  
> None of the upcoming sets of rules and 'best practices' are final and are open for suggestions or own experiences. Feel free to create Issues regarding this topic. Furthermore we try as much as possible to maintain a clean and maintainable project, but we're still an unexperienced team and have a lot to learn.

### Style Guidelines for Unreal Engine 5
We follow the *Gamemakin LLC* style guidelines, which can be found [here](https://github.com/Allar/ue5-style-guide). To simplify compliance with the rules the [Linter v2](https://www.unrealengine.com/marketplace/en-US/product/linter-v2?sessionInvalidated=true) plugin is available for UE4, but unfortunately not yet for version *UE5*. As soon as this is the case, we'll make use of it in our project.

#### Additional Asset Name Modifiers
In addition to the previously mentioned guidelines, we make use of the following project-specific asset name modifiers:

| Asset Type              | Prefix     | Suffix     | Notes                            |
| ----------------------- | ---------- | ---------- | -------------------------------- |
| Gameplay Ability        | GA_        |            |                                  |
| Gameplay Cue            | GC_        |            |                                  |
| Gameplay Effect         | GE_        |            |                                  |

### Coding Guidelines
For our codebase we follow the coding standard provided by the developers of the Unreal Engine, which can be found [here](https://docs.unrealengine.com/5.0/en-US/epic-cplusplus-coding-standard-for-unreal-engine/).

#### Additional Naming Conventions
- Names of project specific data types (user-defined, derived) should include the `PL` prefix following after the Unreal Engine Type prefix, e.g. `APLCharacter`.
- Header and source files should start with the project specific `PL` prefix, e.g. `PLCharacter.h/cpp`.

### Git and GitHub Guidelines
#### Commit Messages
Our commit messages should follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) style, allowing a more structured commit history. In addition we have established other rules that fit our project well:
- The commit message `<type>` should be followed by the `[optional scope]`, which refers to the top-level directory and subdirectory (separated by a hyphen `-`)  related to the change, e.g. `feat(maps):` or `fix(source-core):`. In case of general or unspecified changes the `[optional scope]` can be omitted.
- Under the commit message an `[optional subject]` of `-file`, `-class` or `-asset` is followed by the related name, which is involved in the commit, e.g.
```
feat(source-core): Increases max velocity when walking

-class: APLCharacter
```
- Multiple subjects of the `[optional subject]` can be separated by a comma.

#### Branching
When implementing new features, fixing bugs, etc. create `feature` branches, which use the `develop` branch as their parent. The `main` branch is locked and only used for milestones / releases.

#### Pull Requests
With the development of a new feature, etc. a *pull request* should be created, as soon as the first commit is done. Beforehand it's not possible, because GitHub only let's you create pull requests when changes to the project appear.

Doing so gives us the opportunity to offer suggestions for changes, review, point out errors and misunderstandings, etc. early and collects everything on a single page.

The pull-request should be created/set as "*Draft*" and "*Draft:*" added at the start of the title, until the work on the feature is done.
