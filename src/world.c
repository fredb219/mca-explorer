#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "world.h"
#include "region.h"


static char *basePath = "/.minecraft/saves/";

void world_open(char *name, World *world) {
    char *rootPath;
    char * regionPath;
    char **regionFileList;
    int i;
    
    rootPath = string_cat(getenv("HOME"), basePath);
    
    world->name = string_clone(name);
    world->path = string_cat(rootPath, name);
    
    free(rootPath);
     
    if(!file_exists(world->path)) { 
        printf("World '%s' not found at %s\n", name, world->path);
        return;
    }
    
    regionPath = string_cat(world->path, "/region");
    regionFileList = file_listDir(regionPath, 1, &(world->regionCount));

    
    
    /* Analyse regions*/
    
    world->regions = smalloc(sizeof(Region) * world->regionCount);
    
    for(i = 0; i < world->regionCount; i++) {
        //printf("Region '%s' found\n", regionFileList[i]);
        world->regions[i] = smalloc(sizeof(Region));
        region_init(world->regions[i], regionFileList[i], regionPath);
    }
    
    free(regionPath);
    array_free(regionFileList, world->regionCount);
}

void world_close(World *world) {
    int i;
    free(world->name);
    free(world->path);
    
    for(i = 0; i < world->regionCount; i++) {
        region_destroy(world->regions[i]);
        free(world->regions[i]);
    }
    free(world->regions);
    world->regions = NULL;
    world->regionCount = 0;
    
    world->name = 0;
    world->path = 0;
}

void world_findBlock(World *world, BlockType blockType, Space space) {
    int i;
    
    for(i = 0; i < world->regionCount; i++) {
        if(space_intersect(world->regions[i]->space, space)) { 
            region_findBlock(world->regions[i], blockType, space) ;
        }
    }
}

Space world_getGlobalSpace(World *world) {
    Space space;
    
        (void) world;
    
    space.minX = 0;
    space.minY = 0;
    space.minZ = 0;
    space.maxX = 0;
    space.maxY = 0;
    space.maxZ = 0;
    return space;
}


char space_intersect(Space s1, Space s2) {
    if(s1.maxX < s2.minX || s1.maxY < s2.minY ||  s1.maxZ < s2.minZ) {
        return 0;
    }
    
    if(s2.maxX < s1.minX || s2.maxY < s1.minY ||  s2.maxZ < s1.minZ) {
        return 0;
    }
    
    return 1;
}




char *block_toString(BlockType blockType) {
    switch(blockType) {
        case BLOCK_AIR:
            return "Air";
        case BLOCK_STONE:
            return "Stone";
        case BLOCK_GRASS:
            return "Grass";
        case BLOCK_DIRT:
            return "Dirt";
        case BLOCK_COBBLESTONE:
            return "Cobblestone";  
        case BLOCK_WOODEN_PLANKS:
            return "Wooden Planks";
        case BLOCK_SAPLINGS:
            return "Saplings";    
        case BLOCK_BEDROCK:
            return "Bedrock";     
        case BLOCK_GOLD_ORE:
            return "Gold Ore";     
        case BLOCK_IRON_ORE:
            return "Iron Ore"; 
        case BLOCK_COAL_ORE:
            return "Coal Ore";
        case BLOCK_WOOD:
            return "Wood";  
        case BLOCK_DIAMOND_ORE:
            return "Diamond Ore";
        case BLOCK_CRAFTING_TABLE:
            return "Crafting table";
        case BLOCK_SNOW :
            return "Snow";
        default:
            return "Unknown block";
    }
}


BlockType block_fromString(char *string) {
    for(int i = 0; i < 256; i++) {
        if(strcmp(block_toString(i), string) == 0) {
            return i;
        }
    }
    return -1;
}

