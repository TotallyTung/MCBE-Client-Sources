#pragma once

#

class ChunkPos : public glm::ivec2
{
public:
    ChunkPos() : glm::ivec2(0, 0) {};
    ChunkPos(int x, int z) : glm::ivec2(x, z) {}

    explicit ChunkPos(const glm::vec3& vec)
    {
        const auto blockPos = BlockPos(vec);
        this->x = blockPos.x >> 4;
        this->y = blockPos.z >> 4;
    }

    explicit ChunkPos(const BlockPos& blockPos)
    {
        this->x = blockPos.x >> 4;
        this->y = blockPos.z >> 4;
    }

    bool operator==(const ChunkPos& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    [[nodiscard]] size_t hash() const
    {
        return std::hash<int>()(this->x) ^ std::hash<int>()(this->y);
    }
};

template<>
struct std::hash<ChunkPos>
{
    size_t operator()(const ChunkPos& obj) const noexcept
    {
        return obj.hash();
    }
};

class ChunkBlockPos : public glm::ivec3
{
public:
    explicit ChunkBlockPos(const BlockPos& blockPos)
    {
        this->x = blockPos.x % 16;
        this->y = blockPos.y + 64;
        this->z = blockPos.z % 16;
    }
};

class ChunkSource {
public:
    PAD(0x18);
    class Level*                                                  level;          // this+0x0020
    class Dimension*                                              dimension;      // this+0x0028
    PAD(0x40);
    std::unordered_map<ChunkPos, std::weak_ptr<class LevelChunk>> chunkStorage;   // this+0x0070

    enum LoadMode : int32_t {
        None = 0,
        Deferred = 1
    };

    virtual ~ChunkSource() = default;
    virtual void Function1();
    virtual void Function2();
    virtual void Function3();
    virtual void Function4();
    virtual bool isChunkKnown(const ChunkPos& chunkPos) const;
    virtual void Function6();
    virtual void Function7();
    virtual std::shared_ptr<LevelChunk> getOrLoadChunk(const ChunkPos& chunkPos, LoadMode mode, bool param_3) const;
    virtual void Function9();
    virtual void Function10();
    virtual void Function11();
    virtual void Function12();
    virtual void Function13();
    virtual void Function14();
    virtual void Function15();
    virtual void Function16();
    virtual void Function17();
    virtual void Function18();
    virtual void Function19();
    virtual void Function20();
    virtual void Function21();
    virtual void Function22();
    virtual void Function23();
    virtual const std::unordered_map<ChunkPos, std::weak_ptr<LevelChunk>>& getStorage() const ;
};