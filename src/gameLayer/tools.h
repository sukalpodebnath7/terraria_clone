#pragma once

struct Tool {

    public enum ItemType
    {
        // Wood Tier Tools
        WoodPickaxe = 70,
        WoodAxe = 71,
        WoodHammer = 72,

        // Stone Tier Tools
        StonePickaxe = 73,
        StoneAxe = 74,
        StoneHammer = 75,

        // Copper Tier Tools
        CopperPickaxe = 76,
        CopperAxe = 77,
        CopperHammer = 78,

        // Iron Tier Tools
        IronPickaxe = 79,
        IronAxe = 80,
        IronHammer = 81,

        // Gold Tier Tools
        GoldPickaxe = 82,
        GoldAxe = 83,
        GoldHammer = 84,

        // Materials / Ores / Ingots
        GreenSlimeBall = 85,
        CopperIngot = 86,
        IronIngot = 87,
        GoldIngot = 88,

        // Swords
        WoodSword = 89,
        StoneSword = 90,
        CopperSword = 91,
        IronSword = 92,
        GoldSword = 93,

        // Magic / Orbs / Staves
        RedOrb = 94,
        WoodenStaff = 95,
        GreenGemStaff = 96,
        GoldGemStaff = 97,
        BlueGemOrb = 98,
        BlueGemStaff = 99,

        // Consumables & Miscellaneous
        HealthPotion = 100,
        Apple = 101,
        Scroll = 102,
        Book = 103,
        Bone = 104,
        Shuriken = 105,

        // Daggers
        CopperDagger = 106,
        StoneDagger = 107,
        IronDagger = 108,

        // Wood / Leather Armor Set
        WoodChestplate = 109,
        WoodHelmet = 110,
        WoodBoots = 111,

        // Stone Armor Set
        StoneChestplate = 112,
        StoneHelmet = 113,
        StoneBoots = 114,

        // Copper Armor Set
        CopperChestplate = 115,
        CopperHelmet = 116,
        CopperBoots = 117,

        // Combat Accessories / Special Gear
        RedSlashEffect = 118,
        BowString = 119,
        IronChestplate = 120,
        IronHelmet = 121,
        IronBoots = 122
    }


     int type;

};
