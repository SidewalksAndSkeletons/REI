#pragma once

class IGameObject
{
public:
    IGameObject() = default;
    virtual ~IGameObject() = default;

public:
    virtual void Update() = 0;
    virtual void Render() = 0;
};
