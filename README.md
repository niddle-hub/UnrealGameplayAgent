# GameplayAgent

An Unreal Engine 5 plugin that brings **LLM-powered AI agents into gameplay**. Each agent is driven by a language model that runs **locally** (on-device `Llama`) or via a **remote endpoint**, letting non-player characters understand natural language, reason about their situation, and act in the world — live, at runtime, while interacting with the player.

> **Status: Work in Progress.** This is an active development plugin — APIs and behavior may change without notice.

## Dependencies

- **Llama-Unreal** — the llama.cpp binding used to run models locally (or via a remote endpoint): https://github.com/getnamo/Llama-Unreal

## What it does

A gameplay agent is a data-driven bot. An `AAIController` owns the agent's "brain" — an orchestrator component, an LLM session, and optional voice capture — while the pawn itself stays a plain character. The player talks to the agent through chat, the console, or a dialogue HUD; **voice input is transcribed the same way** and fed through the exact same pipeline.

The agent understands what the player means and responds in-character, either with a plain text reply or by actually doing something in the level: moving, following, turning to face, waiting, speaking, or spawning items.

## Key ideas

- **LLM, not a script.** Agents are model-driven. No hardcoded dialogue trees or canned behavior — the model decides how to respond and act.
- **Hallucination-safe actions.** The model picks from a known, deterministic catalog of commands and only fills in the parameters — it never invents arbitrary actions. Anything it chooses is validated and falls back gracefully if it gets it wrong.
- **Data-driven agents.** Persisted identities, model configs, command catalogs, prompt templates, memory, and toolsets are authored as assets, so agents are easy to configure without rebuilding code.
- **A bounded decision loop.** The agent can chain tool use — act, observe the result, decide again — until it reaches a final text answer or its step limit.
- **Persistent conversation memory.** Chat history and action logs are saved and restored, so the agent remembers across sessions.
- **Connected to the world.** The agent's model is given its persona, what it perceives in the level, its memory, and its constraints — all folded into the prompt so it acts in context.
- **Safe and deterministic.** It only ever executes on the game thread, never fails silently, and always has a reliable fallback when the model is uncertain.

## Getting started

1. Enable the **GameplayAgent** plugin and its **Llama** dependency in your project.
2. Author the agent's identity, model configuration, command catalog, prompt template, and toolset assets.
3. Place an agent character in a level and possess it with its controller.
4. Talk to it. It responds and acts, in real time.

## Status

**Work in progress.** The runtime core and editor tooling are in place. Richer episodic memory, session introspection, VRAM/shared-model pooling, a debug overlay, a dialogue HUD, and networking are planned but not yet implemented.

## License

Released under the [MIT License](LICENSE).