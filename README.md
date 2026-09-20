# GameplayAgent

An Unreal Engine plugin providing a gameplay agent framework and supporting editor tooling.

## Overview

GameplayAgent is a modular plugin that combines an in-engine **agent runtime** (`GameplayAgent`) with **editor tooling** (`GameplayAgentEditor`). The runtime module wires agent identities, model definitions, memory, perception, and toolsets into `AGameplayAgentComponent`, while the editor module extends the editor to export and inspect toolset definitions.

## Requirements

- Unreal Engine 5
- The **Llama** plugin (enabled via the `.uplugin` dependency) provides the LLM-backed modules `LlamaCore`, `LlamaTools`, and `LlamaWhisper` that the runtime depends on.

## Modules

| Module            | Type   | Loading Phase | Purpose                                                        |
| ----------------- | ------ | ------------- | -------------------------------------------------------------- |
| `GameplayAgent`   | Runtime| `Default`     | Agent components, data assets (identity, model, memory, toolset), settings. |
| `GameplayAgentEditor` | Editor | `PostEngineInit` | Editor tooling, including the toolset exporter. |

## Runtime Dependencies

`GameplayAgent` depends on:
- `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `DeveloperSettings`
- `LlamaCore`, `LlamaTools`, `LlamaWhisper`
- `AIModule`, `NavigationSystem`
- `Json`, `JsonUtilities`

## Source Layout

```
Source/
  GameplayAgent/        Runtime module
    Public/Core/        Public headers
      Components/       GameplayAgentComponent, GameplayAgentPerception
      Data/             GameplayAgentIdentity, GameplayAgentModelDefinition,
                        GameplayAgentMemory, GameplayAgentToolset
      Settings/         GameplayAgentSettings
    Private/Core/       Runtime implementation
    Private/Test/       Runtime tests (toolset)
    Public/Test/        Test headers
  GameplayAgentEditor/  Editor module
    Public/
    Private/            Editor implementation, toolset exporter
```

## Configuration

Runtime configuration is read from `Config/DefaultGame.ini`. The plugin registers `GameplayAgentIdentity` and `GameplayAgentModelDefinition` as primary asset types, and configures agents via the `GameplayAgentSettings` section (for example, `GlobalAgentToolset`).

## Getting Started

1. Enable the plugin **and** the Llama plugin in the target project.
2. Add `GameplayAgent` to the project's module dependencies.
3. Drop an `AGameplayAgentComponent` onto an actor in a level and configure the identity, model definition, and toolset assets.
4. (Editor) Use `GameplayAgentEditor`'s toolset exporter to inspect or serialize toolset definitions.

## License

All rights reserved unless otherwise stated.