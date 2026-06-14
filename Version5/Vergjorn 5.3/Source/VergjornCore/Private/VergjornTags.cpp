#include "VergjornTags.h"

// Resources
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Raw_Wood,              "Resource.Raw.Wood",              "Raw timber from felled trees")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Raw_BogIron,           "Resource.Raw.BogIron",           "Iron ore gathered from bog pits")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Refined_Charcoal,      "Resource.Refined.Charcoal",      "Charcoal produced in kilns")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Refined_IronIngot,     "Resource.Refined.IronIngot",     "Smelted iron ingot")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Crafted_Tool_Axe,      "Resource.Crafted.Tool.Axe",      "Woodcutter's axe")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Crafted_Weapon_Sword,  "Resource.Crafted.Weapon.Sword",  "Viking sword")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Crafted_Weapon_Shield, "Resource.Crafted.Weapon.Shield", "Round shield")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Food_Fish,             "Resource.Food.Fish",             "Fish")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resource_Food_Grain,            "Resource.Food.Grain",            "Grain from farms")

// Classes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Class_Thrall, "Class.Thrall", "Enslaved laborer acquired through raids")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Class_Karl,   "Class.Karl",   "Free villager — the backbone of the settlement")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Class_Jarl,   "Class.Jarl",   "Jarl leader unlocking mead hall and raids")

// Occupations
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Lumberjack, "Occupation.Lumberjack", "Fells trees for wood")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_BogWorker,  "Occupation.BogWorker",  "Digs bog iron")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Fisher,     "Occupation.Fisher",     "Fishes rivers and coastlines")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Smelter,    "Occupation.Smelter",    "Smelts iron ingots from ore and charcoal")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Blacksmith, "Occupation.Blacksmith", "Forges tools and weapons")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Farmer,     "Occupation.Farmer",     "Tends crops and produces grain")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Shipwright, "Occupation.Shipwright", "Builds and maintains ships")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Viking,     "Occupation.Viking",     "Armed warrior — Karl + weapon + shield")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Hauler,     "Occupation.Hauler",     "Transports resources between sites")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Occupation_Builder,    "Occupation.Builder",    "Constructs buildings")

// Equipment — Tool slot
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Tool_Axe,        "Equipment.Tool.Axe",        "Woodcutter's axe (+50% chop rate)")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Tool_Pickaxe,    "Equipment.Tool.Pickaxe",    "Iron pickaxe (+50% bog dig rate)")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Tool_FishingRod, "Equipment.Tool.FishingRod", "Fishing rod (+40% fish rate)")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Tool_Scythe,     "Equipment.Tool.Scythe",     "Scythe for harvesting crops (+50% farm rate)")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Tool_Hammer,     "Equipment.Tool.Hammer",     "Smithing/building hammer (+30% craft & build rate)")

// Equipment — Weapon slot
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Weapon_Sword,  "Equipment.Weapon.Sword",  "Viking sword — required for Viking occupation")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Weapon_Shield, "Equipment.Weapon.Shield", "Round shield — required for Viking occupation")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Equipment_Weapon_Spear,  "Equipment.Weapon.Spear",  "Spear — lighter alternative to sword")

// Messages
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Economy_ResourceChanged,    "Message.Economy.ResourceChanged",    "Resource ledger entry changed")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Build_Completed,            "Message.Build.Completed",            "Building construction completed")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Population_VillagerSpawned, "Message.Population.VillagerSpawned", "New villager entity spawned")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Raid_Returned,              "Message.Raid.Returned",              "Raid party returned with loot")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Time_NewDay,                "Message.Time.NewDay",                "New in-game day started")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Time_SeasonChanged,         "Message.Time.SeasonChanged",         "Season transitioned")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Job_Posted,                 "Message.Job.Posted",                 "New job posted to the job board")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Message_Job_Completed,              "Message.Job.Completed",              "Job completed by a villager")

// Jobs
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_Chop,   "Job.Chop",   "Chop a tree node")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_Haul,   "Job.Haul",   "Haul resources to a destination")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_Build,  "Job.Build",  "Work on a construction site")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_Craft,  "Job.Craft",  "Run a production recipe at a workshop")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_DigBog, "Job.DigBog", "Dig bog iron at a bog pit")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_Farm,   "Job.Farm",   "Tend crops at a farm")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Job_Fish,   "Job.Fish",   "Go fishing")

// Building categories
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Building_Category_Storage,    "Building.Category.Storage",    "Resource storage building")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Building_Category_Production, "Building.Category.Production", "Production / workshop building")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Building_Category_Housing,    "Building.Category.Housing",    "Housing for villagers")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Building_Category_Military,   "Building.Category.Military",   "Military training building")

// Seasons
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Season_Spring, "Season.Spring", "Spring season")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Season_Summer, "Season.Summer", "Summer season")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Season_Autumn, "Season.Autumn", "Autumn season")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Season_Winter, "Season.Winter", "Winter season")

// Narrative event IDs
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Event_Intro,           "Narrative.Event.Intro",           "Opening monologue when the player first starts")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Event_FirstTree,       "Narrative.Event.FirstTree",       "Advisor comment when first tree is chopped")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Event_FirstBuilding,   "Narrative.Event.FirstBuilding",   "Advisor comment when first building completes")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Event_FirstWinter,     "Narrative.Event.FirstWinter",     "Warning at the onset of the first winter")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Event_FirstRaid,       "Narrative.Event.FirstRaid",       "Announcement when enemies are spotted for the first time")

// Narrative progression flags
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Flag_IntroSeen,    "Narrative.Flag.IntroSeen",    "Intro event has been acknowledged by the player")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Flag_HasShelter,   "Narrative.Flag.HasShelter",   "At least one house has been built")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Narrative_Flag_WinterWarned, "Narrative.Flag.WinterWarned", "Player has received the first-winter warning")
