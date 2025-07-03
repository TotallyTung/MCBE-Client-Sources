class StartDestroyBlockEvent : public Event
{
public:
    Vector3<int> blockPos;
    unsigned char side;
    StartDestroyBlockEvent(Vector3<int> const& blockPos, unsigned char side)
    {
        this->blockPos = blockPos;
        this->side = side;
    }

    EventType getType() const override { return EventType::StartDestroyBlock; }
};