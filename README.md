<h1 align="center">mkshot-z</h1>

>[!WARNING]
> **WIP.**<br>
> While the build files are present, they are unfinished. The project is in a non-buildable state.

Experimental OneShot (2016) engine reimplementation for modders, derived from [ModShot (mkxp-z)][mshot].

## Why?
The original engine OneShot uses (RPG Maker XP / RMXP) is nearly two decades old. It's old, slow and rather sucky.<br>
Its functionality is too restrictive for extensive modding, and its architecture is ill-suited for modern use cases.

Existing RMXP runtime reimplementations don't improve the situation. Furthermore, they're not suited for modding OneShot in particular.<br>
Using [mkxp-oneshot][xpshot] for your mods is greatly discouraged: it was made for the game specifically and extending upon it will be really difficult; not only that, you won't even be able to build it without manual intervention (Issue [#36](https://github.com/elizagamedev/mkxp-oneshot/issues/36)).

This project's goal is to provide a modern, feature-complete and extensible successor to [mkxp-oneshot][xpshot].<br>
[ModShot (mkxp-z)][mshot] was the chosen one for this, but it's no longer worked on, hence why we use it as the base.

## Usage
Sorry, but this project isn't in the state where it should be used. Stay tuned...

<!--
Instead of stable releases and versioning, **mkshot-z** has autobuilds with a short commit hash.

Get one [here](https://github.com/reverium/mkshot-z/releases/latest) and consult the [documentation](https://github.com/reverium/mkshot-z/wiki) for more info.
--->

## Contributing
Contributions are welcome. In case you're interested, the current development roadmap and priority tasks are at [TODO](TODO).

## License
**mkshot-z** is free and open-source software licensed under the **GPLv3 or later**. See [LICENSE](LICENSE) for more details.

## Credits
- [@thehatkid](https://github.com/thehatkid): For [ModShot (mkxp-z)][mshot].
- [mkxp-z contributors][xpz]: For the [ModShot (mkxp-z)][mshot] base.
- [mkxp-oneshot contributors][xpshot]: For the original OneShot engine implementation.
- [Ancurio & mkxp contributors][xp]: For the [mkxp-oneshot][xpshot] base.

[mshot]: https://github.com/thehatkid/ModShot-mkxp-z
[xpz]: https://github.com/mkxp-z/mkxp-z
[xpshot]: https://github.com/elizagamedev/mkxp-oneshot
[xp]: https://github.com/Ancurio/mkxp
