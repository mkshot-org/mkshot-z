/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#include "core/gfx/tex-pool.hpp"
#include "util/exception.hpp"
#include "core/shared-state.hpp"
#include "core/gfx/state.hpp"
#include "util/boost-hash.hpp"
#include "util/dbg-writer.hpp"

#include <list>
#include <utility>
#include <assert.h>
#include <string.h>

typedef std::pair<uint16_t, uint16_t> Size;

static uint32_t byteCount(Size &s)
{
	return s.first * s.second * 4;
}

struct CacheNode
{
	TEXFBO obj;
	std::list<TEXFBO>::iterator prioIter;

	bool operator==(const CacheNode &o) const
	{
		return obj == o.obj;
	}
};

typedef std::list<CacheNode> CNodeList;

struct TexPoolPrivate
{
	/* Contains all cached TexFBOs, grouped by size */
	BoostHash<Size, CNodeList> poolHash;

	/* Contains all cached TexFBOs, sorted by release time */
	std::list<TEXFBO> priorityQueue;

	/* Maximal allowed cache memory */
	const uint32_t maxMemSize;

	/* Current amound of memory consumed by the cache */
	uint32_t memSize;

	/* Current amount of TexFBOs cached */
	uint16_t objCount;

	/* Has this pool been disabled? */
	bool disabled;

	TexPoolPrivate(uint32_t maxMemSize)
	    : maxMemSize(maxMemSize),
	      memSize(0),
	      objCount(0),
	      disabled(false)
	{}
};

TexPool::TexPool(uint32_t maxMemSize)
{
	p = new TexPoolPrivate(maxMemSize);
}

TexPool::~TexPool()
{
	std::list<TEXFBO>::iterator iter;

	for (iter = p->priorityQueue.begin();
	     iter != p->priorityQueue.end();
	     ++iter)
	{
		TEXFBO obj = *iter;
		TEXFBO::fini(obj);
		--p->objCount;
	}

	assert(p->objCount == 0);

	delete p;
}

TEXFBO TexPool::request(int width, int height)
{
	CacheNode cnode;
	Size size(width, height);

	/* See if we can statisfy request from cache */
	CNodeList &bucket = p->poolHash[size];

	if (!bucket.empty())
	{
		/* Found one! */
		cnode = bucket.back();
		bucket.pop_back();

		p->priorityQueue.erase(cnode.prioIter);

		p->memSize -= byteCount(size);
		--p->objCount;

//		Debug() << "TexPool: <?+> (" << width << height << ")";

		return cnode.obj;
	}

	int maxSize = glState.caps.maxTexSize;
	if (width > maxSize || height > maxSize)
		throw Exception(Exception::MKShotError,
		                "Texture dimensions [%d, %d] exceed hardware capabilities",
		                width, height);

	/* Nope, create it instead */
	TEXFBO::init(cnode.obj);
	TEXFBO::allocEmpty(cnode.obj, width, height);
	TEXFBO::linkFBO(cnode.obj);

//	Debug() << "TexPool: <?-> (" << width << height << ")";

	return cnode.obj;
}

void TexPool::release(TEXFBO &obj)
{
	if (obj.tex == TEX::ID(0) || obj.fbo == FBO::ID(0))
	{
		TEXFBO::fini(obj);
		return;
	}

	if (p->disabled)
	{
		/* If we're disabled, delete without caching */
//		Debug() << "TexPool: <!#> (" << obj.width << obj.height << ")";
		TEXFBO::fini(obj);
		return;
	}

	Size size(obj.width, obj.height);

	uint32_t newMemSize = p->memSize + byteCount(size);

	/* If caching this object would spill over the allowed memory budget,
	 * delete least used objects until we're good again */
	while (newMemSize > p->maxMemSize)
	{
		if (p->objCount == 0)
			break;

//		Debug() << "TexPool: <!~> Size:" << p->memSize;

		/* Retrieve object with lowest priority for deletion */
		CacheNode last;
		last.obj = p->priorityQueue.back();
		Size removedSize(last.obj.width, last.obj.height);

		CNodeList &bucket = p->poolHash[removedSize];

		std::list<CacheNode>::iterator toRemove =
		        std::find(bucket.begin(), bucket.end(), last);
		assert(toRemove != bucket.end());
		bucket.erase(toRemove);

		p->priorityQueue.pop_back();

		TEXFBO::fini(last.obj);

		newMemSize -= byteCount(removedSize);
		--p->objCount;

//		Debug() << "TexPool: <!-> (" << last.obj.width << last.obj.height << ")";
	}

	p->memSize = newMemSize;

	/* Retain object */
	p->priorityQueue.push_front(obj);
	CacheNode cnode;
	cnode.obj = obj;
	cnode.prioIter = p->priorityQueue.begin();
	CNodeList &bucket = p->poolHash[size];
	bucket.push_back(cnode);

	++p->objCount;

//	Debug() << "TexPool: <!+> (" << obj.width << obj.height << ") Current size:" << p->memSize;
}

void TexPool::disable()
{
	p->disabled = true;
}


