
#pragma once

#include "BlockHandler.h"
#include "Mixins/DirtLikeUnderneath.h"
#include "ChunkInterface.h"





/** Handles the grass that is 1 block tall */
class cBlockTallGrassHandler final :
	public cDirtLikeUnderneath<cBlockHandler>
{
	using Super = cDirtLikeUnderneath<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, const NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		return true;
	}





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// If using shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(m_BlockType, 1, a_BlockMeta);
		}

		// Drop seeds, depending on bernoulli trial result:
		if (GetRandomProvider().RandBool(0.875))  // 87.5% chance of dropping nothing
		{
			return {};
		}

		// 12.5% chance of dropping 0 or more seeds.
		const auto DropNum = FortuneDiscreteRandom(1, 1, 2 * ToolFortuneLevel(a_Tool));
		return cItem(E_ITEM_SEEDS, DropNum);
	}





	/** Growing a tall grass produces a big flower (2-block high fern or double-tall grass). */
	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		const auto TopPos = a_RelPos.addedY(1);
		if (!cChunkDef::IsValidHeight(TopPos))
		{
			return 0;
		}
		auto blockMeta = a_Chunk.GetMeta(a_RelPos);
		NIBBLETYPE largeFlowerMeta;
		switch (blockMeta)
		{
			case E_META_TALL_GRASS_GRASS: largeFlowerMeta = E_META_BIG_FLOWER_DOUBLE_TALL_GRASS; break;
			case E_META_TALL_GRASS_FERN:  largeFlowerMeta = E_META_BIG_FLOWER_LARGE_FERN; break;
			default:                      return 0;
		}
		a_Chunk.SetBlock(a_RelPos,           E_BLOCK_BIG_FLOWER, largeFlowerMeta);
		a_Chunk.SetBlock(TopPos, E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP);
		return 1;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




