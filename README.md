# GameFU Station

A fantasy games console and home computing environment.

The GameFU Station project implements an emulator and a toolchain for a custom MIPS-like architecture and provides the system's kernel and official SDK.

Primary Git mirror on Codeberg: https://codeberg.org/gamefu/gamefu-station

Project contact email: contact@gamefu-station.org
Request subscription to mailing lists: subscribe@gamefu-station.org
Submit patch requests: patch@gamefu-station.org

## Building from source

To build the projects in this repository, bootstrap the build tool by compiling the `nob.c` build source.

```sh
$ cc -o nob nob.c
```

```bat
> cl -o nob.exe nob.c
```

Running the resulting executable will, by default, configure and build all projects.

The build tool comes with a `config` command used to specify compilation options or features.
For example, `--san=address` will enable the address sanitizer as well as any other compatible sanitizers.
Any time the configuration is changed, a full rebuild is triggered.

## Contributing to the GameFU Station project

### Method 1: Codeberg Pull Requests

If you have a Codeberg account and a fork of the GameFU Station project, you can use the Pull Request feature to submit changes for review and potential acceptance.

### Method 2: GameFU Mailing List

If you do not or do not wish to use your Codeberg account, you can submit patches through the GameFU Station mailing list. You can use Git to format patches and also to email them directly from the CLI, or just email the patch file manually when generated.

See documentation on [formatting a patch with Git](https://git-scm.com/docs/git-format-patch) and [sending an email with Git](https://git-scm.com/docs/git-send-email) for more information. You may need to install additional OS packages and look up specific documentation to configure Git's email capabilities.

To create a patch, do some work and then use `git format-patch`:

```sh
$ git checkout -b my-feature-branch
# hack hack hack
$ git commit ...
$ git format-patch big
0001-Add-my-feature.patch
```

If you continue to work, you can update a patch:

```sh
# hack hack hack
$ git commit --amend --reset
$ git format-patch -v2 big
v2-0001-Add-my-feature.patch
```

Submissions can also be multiple commits:

```sh
$ git checkout -b my-feature-branch
# hack hack hack
$ git add ...
$ git commit ...
$ git add ...
$ git commit ...
$ git format-patch big
0001-Some-refactoring.patch
0002-Add-my-feature.patch
```

To send an email containing the patch (once configured for your sender address):

```sh
$ git send-email --to patch@gamefu-station.org [patch-file]
```

(Thanks to [Peter Eisentraut](https://peter.eisentraut.org/blog/2023/05/09/how-to-submit-a-patch-by-email-2023-edition) for the `git format-patch` example commands. Read this post for more details.)
