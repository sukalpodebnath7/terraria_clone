#pragma once
#include <block.h>
#include <tools.h>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

unordered_map<int, vector<pair<int, int>>> recipeBook = {

    // --- WOOD TIER TOOLS ---
    { Tool::WoodPickaxe,       { {Block::woodPlank, 3}, {Block::woodLog, 2} } },
    { Tool::WoodAxe,           { {Block::woodPlank, 3}, {Block::woodLog, 2} } },
    { Tool::WoodHammer,        { {Block::woodPlank, 2}, {Block::woodLog, 2} } },

    // --- STONE TIER TOOLS ---
    { Tool::StonePickaxe,      { {Block::stone, 3}, {Block::woodLog, 2} } },
    { Tool::StoneAxe,          { {Block::stone, 3}, {Block::woodLog, 2} } },
    { Tool::StoneHammer,       { {Block::stone, 2}, {Block::woodLog, 2} } },

    // --- COPPER TIER TOOLS ---
    { Tool::CopperPickaxe,     { {Block::copperBlock, 3}, {Block::woodLog, 2} } },
    { Tool::CopperAxe,         { {Block::copperBlock, 3}, {Block::woodLog, 2} } },
    { Tool::CopperHammer,      { {Block::copperBlock, 2}, {Block::woodLog, 2} } },

    // --- IRON TIER TOOLS ---
    { Tool::IronPickaxe,       { {Block::ironBlock, 3}, {Block::woodLog, 2} } },
    { Tool::IronAxe,           { {Block::ironBlock, 3}, {Block::woodLog, 2} } },
    { Tool::IronHammer,        { {Block::ironBlock, 2}, {Block::woodLog, 2} } },

    // --- GOLD TIER TOOLS ---
    { Tool::GoldPickaxe,       { {Block::goldBlock, 3}, {Block::woodLog, 2} } },
    { Tool::GoldAxe,           { {Block::goldBlock, 3}, {Block::woodLog, 2} } },
    { Tool::GoldHammer,        { {Block::goldBlock, 2}, {Block::woodLog, 2} } },

    // --- MATERIALS / ORES / INGOTS (Uncraftable Base Drops) ---
    { Tool::GreenSlimeBall,    {} },
    { Tool::CopperIngot,       { {Block::copper, 1} } }, // Smelted from raw ore
    { Tool::IronIngot,         { {Block::iron, 1} } },   // Smelted from raw ore
    { Tool::GoldIngot,         { {Block::gold, 1} } },   // Smelted from raw ore

    // --- SWORDS ---
    { Tool::WoodSword,         { {Block::woodPlank, 2}, {Block::woodLog, 1} } },
    { Tool::StoneSword,        { {Block::stone, 2}, {Block::woodLog, 1} } },
    { Tool::CopperSword,       { {Block::copperBlock, 2}, {Block::woodLog, 1} } },
    { Tool::IronSword,         { {Block::ironBlock, 2}, {Block::woodLog, 1} } },
    { Tool::GoldSword,         { {Block::goldBlock, 2}, {Block::woodLog, 1} } },

    // --- MAGIC / ORBS / STAVES ---
    { Tool::RedOrb,            { {Block::rubyBlock, 1}, {Block::glass, 1} } },
    { Tool::WoodenStaff,       { {Block::woodLog, 3} } },
    { Tool::GreenGemStaff,     { {Block::woodLog, 2} }}, // Assuming green jewel replacement or ruby variant
    { Tool::GoldGemStaff,      { {Block::goldBlock, 2}, {Block::rubyBlock, 1} } },
    { Tool::BlueGemOrb,        { {Block::blueRubyBlock, 1}, {Block::glass, 1} } },
    { Tool::BlueGemStaff,      { {Block::ironBlock, 2}, {Block::blueRubyBlock, 1} } },

    // --- CONSUMABLES & MISC ---
    { Tool::HealthPotion,      { {Block::jar, 1}, {Block::grass, 1} } },
    { Tool::Apple,             {} }, // Tree drop
    { Tool::Scroll,            { {Block::woodPlank, 1} } }, // Paper equivalent
    { Tool::Book,              { {Block::woodPlank, 3} } },
    { Tool::Bone,              {} }, // Enemy drop
    { Tool::Shuriken,          { {Block::ironBlock, 1} } },

    // --- DAGGERS ---
    { Tool::CopperDagger,      { {Block::copperBlock, 1}, {Block::woodLog, 1} } },
    { Tool::StoneDagger,       { {Block::stone, 1}, {Block::woodLog, 1} } },
    { Tool::IronDagger,        { {Block::ironBlock, 1}, {Block::woodLog, 1} } },

    // --- WOOD / LEATHER ARMOR SET ---
    { Tool::WoodChestplate,    { {Block::woodPlank, 8} } },
    { Tool::WoodHelmet,        { {Block::woodPlank, 5} } },
    { Tool::WoodBoots,         { {Block::woodPlank, 4} } },

    // --- STONE ARMOR SET ---
    { Tool::StoneChestplate,   { {Block::stone, 8} } },
    { Tool::StoneHelmet,       { {Block::stone, 5} } },
    { Tool::StoneBoots,        { {Block::stone, 4} } },

    // --- COPPER ARMOR SET ---
    { Tool::CopperChestplate,  { {Block::copperBlock, 8} } },
    { Tool::CopperHelmet,      { {Block::copperBlock, 5} } },
    { Tool::CopperBoots,       { {Block::copperBlock, 4} } },

    // --- COMBAT ACCESSORIES / SPECIAL GEAR ---
    { Tool::RedSlashEffect,    {} }, // Visual effect, not craftable
    { Tool::BowString,         { {Block::grass, 3} } }, // Fiber/String craft
    { Tool::IronChestplate,    { {Block::ironBlock, 8} } },
    { Tool::IronHelmet,        { {Block::ironBlock, 5} } },
    { Tool::IronBoots,         { {Block::ironBlock, 4} } }

};