#include <pmm.h>
#include <list.h>
#include <string.h>
#include <default_pmm.h>

/*  In the First Fit algorithm, the allocator keeps a list of free blocks
 * (known as the free list). Once receiving a allocation request for memory,
 * it scans along the list for the first block that is large enough to satisfy
 * the request. If the chosen block is significantly larger than requested, it
 * is usually splitted, and the remainder will be added into the list as
 * another free block.
 *  Please refer to Page 196~198, Section 8.2 of Yan Wei Min's Chinese book
 * "Data Structure -- C programming language".
*/
// LAB2 EXERCISE 1: YOUR CODE
// you should rewrite functions: `default_init`, `default_init_memmap`,
// `default_alloc_pages`, `default_free_pages`.
/*
 * Details of FFMA
 * (1) Preparation:
 *  In order to implement the First-Fit Memory Allocation (FFMA), we should
 * manage the free memory blocks using a list. The struct `free_area_t` is used
 * for the management of free memory blocks.
 *  First, you should get familiar with the struct `list` in list.h. Struct
 * `list` is a simple doubly linked list implementation. You should know how to
 * USE `list_init`, `list_add`(`list_add_after`), `list_add_before`, `list_del`,
 * `list_next`, `list_prev`.
 *  There's a tricky method that is to transform a general `list` struct to a
 * special struct (such as struct `page`), using the following MACROs: `le2page`
 * (in memlayout.h), (and in future labs: `le2vma` (in vmm.h), `le2proc` (in
 * proc.h), etc).
 * (2) `default_init`:
 *  You can reuse the demo `default_init` function to initialize the `free_list`
 * and set `nr_free` to 0. `free_list` is used to record the free memory blocks.
 * `nr_free` is the total number of the free memory blocks.
 * (3) `default_init_memmap`:
 *  CALL GRAPH: `kern_init` --> `pmm_init` --> `page_init` --> `init_memmap` -->
 * `pmm_manager` --> `init_memmap`.
 *  This function is used to initialize a free block (with parameter `addr_base`,
 * `page_number`). In order to initialize a free block, firstly, you should
 * initialize each page (defined in memlayout.h) in this free block. This
 * procedure includes:
 *  - Setting the bit `PG_property` of `p->flags`, which means this page is
 * valid. P.S. In function `pmm_init` (in pmm.c), the bit `PG_reserved` of
 * `p->flags` is already set.
 *  - If this page is free and is not the first page of a free block,
 * `p->property` should be set to 0.
 *  - If this page is free and is the first page of a free block, `p->property`
 * should be set to be the total number of pages in the block.
 *  - `p->ref` should be 0, because now `p` is free and has no reference.
 *  After that, We can use `p->page_link` to link this page into `free_list`.
 * (e.g.: `list_add_before(&free_list, &(p->page_link));` )
 *  Finally, we should update the sum of the free memory blocks: `nr_free += n`.
 * (4) `default_alloc_pages`:
 *  Search for the first free block (block size >= n) in the free list and resize
 * the block found, returning the address of this block as the address required by
 * `malloc`.
 *  (4.1)
 *      So you should search the free list like this:
 *          list_entry_t le = &free_list;
 *          while((le=list_next(le)) != &free_list) {
 *          ...
 *      (4.1.1)
 *          In the while loop, get the struct `page` and check if `p->property`
 *      (recording the num of free pages in this block) >= n.
 *              struct Page *p = le2page(le, page_link);
 *              if(p->property >= n){ ...
 *      (4.1.2)
 *          If we find this `p`, it means we've found a free block with its size
 *      >= n, whose first `n` pages can be malloced. Some flag bits of this page
 *      should be set as the following: `PG_reserved = 1`, `PG_property = 0`.
 *      Then, unlink the pages from `free_list`.
 *          (4.1.2.1)
 *              If `p->property > n`, we should re-calculate number of the rest
 *          pages of this free block. (e.g.: `le2page(le,page_link))->property
 *          = p->property - n;`)
 *          (4.1.3)
 *              Re-caluclate `nr_free` (number of the the rest of all free block).
 *          (4.1.4)
 *              return `p`.
 *      (4.2)
 *          If we can not find a free block with its size >=n, then return NULL.
 * (5) `default_free_pages`:
 *  re-link the pages into the free list, and may merge small free blocks into
 * the big ones.
 *  (5.1)
 *      According to the base address of the withdrawed blocks, search the free
 *  list for its correct position (with address from low to high), and insert
 *  the pages. (May use `list_next`, `le2page`, `list_add_before`)
 *  (5.2)
 *      Reset the fields of the pages, such as `p->ref` and `p->flags` (PageProperty)
 *  (5.3)
 *      Try to merge blocks at lower or higher addresses. Notice: This should
 *  change some pages' `p->property` correctly.
 */
free_area_t free_area;

#define free_list (free_area.free_list)
#define nr_free (free_area.nr_free)

static void
default_init(void)
{
    list_init(&free_list);
    nr_free = 0;
}

static void
default_init_memmap(struct Page *base, size_t n)
{
    cprintf("  (Ysl)17 base = 0x%08lx, n = %d\n", base, n);
    assert(n > 0);
    struct Page *p = base;
    //A free block contains n pages
    for (; p < base + n; p++)
    {
        assert(PageReserved(p));
        p->flags = p->property = 0;
        set_page_ref(p, 0);
    }
    //Set the first page property of the free block
    base->property = n;
    SetPageProperty(base); //Page free
    nr_free += n;
    list_add(&free_list, &(base->page_link));
    cprintf("  (Ysl)18 0x%08lx.[&free_list], below the addr:0x0011b000.[kernel end]\n", &free_list);
    cprintf("  (Ysl)19 0x%08lx.[&(base->page_link)], addr of base(0x%08lx) + 12 = 0x%08lx\n", &(base->page_link), base, &(base->page_link));

    cprintf("  (Ysl)20 The initialization and setting of free memory page Struct is finished!!\n\n");
}

static struct Page *
default_alloc_pages(size_t n)
{
    assert(n > 0);
    if (n > nr_free)
        return NULL;

    struct Page *page = NULL;
    list_entry_t *le = &free_list;

    while ((le = list_next(le)) != &free_list) //寻找符合要求的空闲块
    {
        struct Page *p = le2page(le, page_link);
        if (p->property >= n)
        {
            page = p;
            break;
        }
    }

    if (page != NULL)
    {
        if (page->property > n) //当前块所拥有页的数量page->property 大于 所请求页数量n
        {
            struct Page *p = page + n;
            p->property = page->property - n;
            SetPageProperty(p); //将当前空闲块的首页从page修改为p
            list_add_after(&(page->page_link), &(p->page_link));
        }
        list_del(&(page->page_link)); //已分配，从内存空闲链表中删除
        nr_free -= n;
        ClearPageProperty(page);
    }
    return page;
}

static void
default_free_pages(struct Page *base, size_t n)
{
    assert(n > 0);

    struct Page *p = base;
    for (; p != base + n; p++) //初始化待回收的n个页：flag=0、ref=0
    {
        assert(!PageReserved(p) && !PageProperty(p));
        p->flags = 0;
        set_page_ref(p, 0);
    }

    base->property = n;
    SetPageProperty(base);
    list_entry_t *le = list_next(&free_list);
    while (le != &free_list) //在加入空闲块链表之前，搜寻是否能够合并，若是则进行合并；由于会对全部空闲块进行遍历，则连续的一定会合并
    {
        p = le2page(le, page_link);
        if (base + base->property == p) //[base p]
        {
            base->property += p->property;
            ClearPageProperty(p);
            list_del(&(p->page_link));
        }
        else if (p + p->property == base) //[p base]
        {
            p->property += base->property;
            ClearPageProperty(base);
            base = p;
            list_del(&(p->page_link));
        }
        le = list_next(le);
    }
    nr_free += n;

    le = list_next(&free_list);
    while (le != &free_list)
    {
        p = le2page(le, page_link);
        if (base + base->property <= p)
        {
            assert(base + base->property != p);
            break;
        }
        le = list_next(le);
    }
    list_add_before(le, &(base->page_link));
}

static size_t
default_nr_free_pages(void)
{
    return nr_free;
}

static void
basic_check(void)
{
    struct Page *p0, *p1, *p2;
    p0 = p1 = p2 = NULL;
    //第一次检测，连续分配三块空间，并检测分配块的属性
    assert((p0 = alloc_page()) != NULL);
    assert((p1 = alloc_page()) != NULL);
    assert((p2 = alloc_page()) != NULL);

    assert(p0 != p1 && p0 != p2 && p1 != p2);
    assert(page_ref(p0) == 0 && page_ref(p1) == 0 && page_ref(p2) == 0);

    assert(page2pa(p0) < npage * PGSIZE);
    assert(page2pa(p1) < npage * PGSIZE);
    assert(page2pa(p2) < npage * PGSIZE);

    //第二次检测，是否能够正常回收
    list_entry_t free_list_store = free_list;
    list_init(&free_list);
    assert(list_empty(&free_list));

    unsigned int nr_free_store = nr_free;
    nr_free = 0;

    assert(alloc_page() == NULL);

    free_page(p0);
    free_page(p1);
    free_page(p2);
    assert(nr_free == 3);

    assert((p0 = alloc_page()) != NULL);
    assert((p1 = alloc_page()) != NULL);
    assert((p2 = alloc_page()) != NULL); //此时nr_free = 0

    assert(alloc_page() == NULL);

    free_page(p0);
    assert(!list_empty(&free_list));

    struct Page *p;
    assert((p = alloc_page()) == p0);
    assert(alloc_page() == NULL);

    assert(nr_free == 0);
    free_list = free_list_store;
    nr_free = nr_free_store;

    free_page(p);
    free_page(p1);
    free_page(p2);
}

// LAB2: below code is used to check the first fit allocation algorithm (your EXERCISE 1)
// NOTICE: You SHOULD NOT CHANGE basic_check, default_check functions!
static void
default_check(void)
{
    int count = 0, total = 0;
    list_entry_t *le = &free_list;
    while ((le = list_next(le)) != &free_list)
    {
        struct Page *p = le2page(le, page_link);
        assert(PageProperty(p));
        count++, total += p->property;
    }
    assert(total == nr_free_pages());

    cprintf("  (Ysl)21 the default_alloc_pages() function performs the following steps: \n");
    cprintf("          1. Search for free blocks that meet the requirements\n");
    cprintf("          2. Modify the free block according to the number of required pages(modify property and link state)\n");
    cprintf("          note: The address of the Page struct is returned, not just the memory space address\n");
    cprintf("                The starting address of memory space corresponding to page can be obtained through conversion\n");

    cprintf("  (Ysl)22 the default_free_pages() function performs the following steps: \n");
    cprintf("          1. Initialize N pages to be recycled: Flag = 0, ref = 0. \n");
    cprintf("          2. Check whether the blocks to be recycled are adjacent to each free block in the free linked list\n");
    cprintf("             If they are adjacent, they will be merged, like this: \n");
    cprintf("             [free] [freeing] [free] -> [free freeing] [free] -> [free freeing free]\n");
    cprintf("          3. Determine the location of the blocks to be recycled in the free list, and then merge them into the free list\n");

    basic_check();
    cprintf("  (Ysl)23 basic_check succeeded!\n");

    struct Page *p0 = alloc_pages(5), *p1, *p2;
    assert(p0 != NULL);
    assert(!PageProperty(p0)); //分配出去了，page的property就为0

    list_entry_t free_list_store = free_list;
    list_init(&free_list);
    assert(list_empty(&free_list));
    assert(alloc_page() == NULL);

    unsigned int nr_free_store = nr_free;
    nr_free = 0;

    free_pages(p0 + 2, 3);
    assert(alloc_pages(4) == NULL);
    assert(PageProperty(p0 + 2) && p0[2].property == 3);
    assert((p1 = alloc_pages(3)) != NULL);
    assert(alloc_page() == NULL);
    assert(p0 + 2 == p1);

    p2 = p0 + 1;
    free_page(p0);
    free_pages(p1, 3);
    assert(PageProperty(p0) && p0->property == 1);
    assert(PageProperty(p1) && p1->property == 3);

    assert((p0 = alloc_page()) == p2 - 1);
    free_page(p0);
    assert((p0 = alloc_pages(2)) == p2 + 1);

    free_pages(p0, 2);
    free_page(p2);

    assert((p0 = alloc_pages(5)) != NULL);
    assert(alloc_page() == NULL);

    assert(nr_free == 0);
    nr_free = nr_free_store;

    free_list = free_list_store;
    free_pages(p0, 5);

    le = &free_list;
    while ((le = list_next(le)) != &free_list)
    {
        struct Page *p = le2page(le, page_link);
        count--, total -= p->property;
    }
    assert(count == 0);
    assert(total == 0);

    cprintf("  (Ysl)27 default_check succeeded!\n");
}

const struct pmm_manager default_pmm_manager = {
    .name = "default_pmm_manager",
    .init = default_init,
    .init_memmap = default_init_memmap,
    .alloc_pages = default_alloc_pages,
    .free_pages = default_free_pages,
    .nr_free_pages = default_nr_free_pages,
    .check = default_check,
};
