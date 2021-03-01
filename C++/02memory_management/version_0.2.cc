#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
内存池模型如下：
MemoryPool -> block -> block -> block -> block -> block -> ...
                |        |        |        |        |    
                v        v        v        v        v    
              chunk    chunk    chunk    chunk    chunk   
                |        |        |        |        |    
                v        v        v        v        v    
              chunk    chunk    chunk    chunk    chunk   
                |        |        |        |        |    
                v        v        v        v        v    
               ...      ...      ...      ...      ...  
*/
typedef struct _chunk
{
    struct _chunk *next;
    int chunk_size;
    char buf[];

} Chunk;

typedef struct _block
{
    int chunk_cnt;
    int chunk_size;
    struct _block *next;
    struct _chunk *chunk_list;
} Block;

typedef struct _memory_pool
{
    int block_cnt;
    struct _block *next;
} MemoryPool;

MemoryPool *mem_pool_create(int chunk_size, int factor)
{
    MemoryPool *mp = (MemoryPool *)malloc(sizeof(MemoryPool));
    if (!mp)
        return NULL;
    mp->block_cnt = 4; //配置
    mp->next = NULL;
    int now_factor = 1;
    Block *last_block = mp->next;
    for (int i = 0; i < mp->block_cnt; i++)
    {
        Block *next_block = (Block *)malloc(sizeof(Block));
        if (!next_block)
        {
            free(mp);
            return NULL;
        }
        if (last_block == NULL)
        {
            mp->next = next_block;
            last_block = next_block;
        }
        else
        {
            last_block->next = next_block;
            last_block = next_block;
        }
        next_block->chunk_cnt = 10;
        next_block->chunk_size = chunk_size * now_factor;
        now_factor *= factor;
        for (int j = 0; j < next_block->chunk_cnt; j++)
        {
            Chunk *next_chunk = next_block->chunk_list;
            Chunk *header_chunk = (Chunk *)malloc(sizeof(Chunk) + next_block->chunk_size);
            header_chunk->next = next_chunk;
            next_block->chunk_list = header_chunk;
            header_chunk->chunk_size = next_block->chunk_size;
            memset(header_chunk->buf, 0, next_block->chunk_size);
        }
    }
    return mp;
}

void *mem_pool_alloc(MemoryPool *pool, int size)
{
    Block *block = pool->next;
    while (block && block->chunk_size < size)
    {
        block = block->next;
    }
    if (!block)
        return NULL;
    Chunk *ret = block->chunk_list;
    if (ret)
    {
        block->chunk_list = block->chunk_list->next;
        printf("mem_pool_alloc %p,%p\n", ret, ret->buf);
        return (char *)ret + sizeof(Chunk);
    }
    return NULL;
}

void mem_pool_free(MemoryPool *pool, void *memory)
{
    Chunk *now = (Chunk *)(memory - sizeof(Chunk));
    int size = now->chunk_size;
    printf("mem_pool_free %d, %p\n", now->chunk_size, now);

    Block *block = pool->next;
    while (block && block->chunk_size < size)
    {
        block = block->next;
    }
    if (!block || block->chunk_size != size)
        return;
    Chunk *old = block->chunk_list;
    block->chunk_list = now;
    block->chunk_list->next = old;
}

int main()
{
    MemoryPool *pool = mem_pool_create(8, 2);
    if (pool == NULL)
    {
        return 0;
    }
    char *buffer = (char *)mem_pool_alloc(pool, 8);
    printf("buffer %p\n", buffer);
    char *buffer2 = (char *)mem_pool_alloc(pool, 8);
    printf("buffer2 %p\n", buffer2);
    if (!buffer || !buffer2)
    {
    }

    strcpy(buffer, "abcdefg\n");
    strcpy(buffer2, "bcdefgh\n");

    printf("%s", buffer);
    printf("%s", buffer2);

    mem_pool_free(pool, buffer);
    mem_pool_free(pool, buffer2);
    //re test
    char *buffer3 = (char *)mem_pool_alloc(pool, 16);
    printf("buffer3 %p\n", buffer3);

    strcpy(buffer3, "abcdefghi\n");
    printf("%s", buffer3);
    mem_pool_free(pool, buffer3);

    //re test
    char *buffer4 = (char *)mem_pool_alloc(pool, 8);
    printf("buffer4 %p\n", buffer4);

    strcpy(buffer4, "abcdefg\n");
    printf("%s", buffer4);
    mem_pool_free(pool, buffer4);

    return 0;
}