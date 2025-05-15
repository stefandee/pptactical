// mission states
const S_INIT          = 0;
const S_TRANSPORT     = 1;
const S_COMBAT        = 2;
const S_END           = 4;
const S_BRIEFING_LAND = 5;
const S_MISSION_FAILED = 6;
const S_MISSION_SUCCESFUL = 7;

// batlegroup constants
const BG_LANDSCAPE         = 1;
const BG_PLAYER            = 2;
const BG_PLAYER_TRANSPORT  = 16;
const BG_RECON             = 32;
const BG_EXTRACT_TRANSPORT = 64;
const BG_ENEMIES           = 128;

// units constants
const INSERTION_CHOPPER_ID1    = 231;
const INSERTION_CHOPPER_ID2    = 232;
const COMM_BUNKER_ID           = 0;

// position constants
const gReconLocation[2]    = { 66, 15 };
const gCommLocation[2]     = { 25, 18 };
const gExtractLocation[2]  = { 37, 18 };

// vars
var state = S_INIT;

// functions
ShowMap(var xs, var ys, var xe, var ye, var value)
{
  var i, j;

  for(i = xs; i < xe; i++)
  {
    for(j = ys; j < ye; j++)
    {
      NotifyVisibilityMap(i, j, BG_PLAYER, value);
    }
  }
}

// changes the battlegroup of units in an area
ChangeBattleGroup(const bgid, const xs, const ys, const xe, const ye)
{
  var unitid;
  var i, j;

  for(i = xs; i < xe; i++)
  {
    for(j = ys; j < ye; j++)
    {
      unitid = GetUnitAtPoint(i, j);

      if (unitid != -1 && GetUnitType(unitid) == 2)
      {
        ChangeUnitBattleGroup(unitid, bgid); 
      }
    }
  }
}

Init()
{
  state = S_TRANSPORT;
}

Transport()
{
  var x = GetUnitLogicX(INSERTION_CHOPPER_ID1);
  var y = GetUnitLogicY(INSERTION_CHOPPER_ID1);

  MoveCamera(x, y);
  ShowMap(x - 6, y - 6, x + 6, y + 6, 2);

  if (UnitsTransportedCount(INSERTION_CHOPPER_ID1) == 0 && UnitsTransportedCount(INSERTION_CHOPPER_ID2) == 0)
  {
    // assign the unloaded units to the player
    ChangeBattleGroup(BG_PLAYER, 41, 57, 59, 69);

    state = S_BRIEFING_LAND;
  }
}

BriefingLand()
{
  PostMessage("Move out and meet the recon team to obtain the location of the communication bunker.");
  MoveCamera(gReconLocation[0], gReconLocation[1]);
  ShowMap(gReconLocation[0] - 4, gReconLocation[1] - 4, gReconLocation[0] + 4, gReconLocation[1] + 4, 2);
  //NotifyCircleVisibilityMap(gReconLocation[0], gReconLocation[1], 6, BG_PLAYER, 2);
  PostMessage("They were last seen in this area.");

  PostMessage("Then move in and assault the bunker.");

  state = S_COMBAT;
}

// checks that a battlegroup still exists (all its unit are alive)
BattleGroupValid(const bgid)
{
  var i;

  var unitsCount = GetBattleGroupUnitCount(bgid);
  var unitsDead  = 0;

  for(i = 0; i < unitsCount; i++)
  {
    if (IsUnitDead(GetBattleGroupUnit(bgid, i)) != 1)
    {
      unitsDead++;
    }
  }

  if (unitsDead == unitsCount)
  {
    return 0;
  }

  return 1;
}

var bunkerAreaCleared = 0;
var missionCountdown = 0;
var countdownFinished = 0;
var metReconTeam      = 0;
var enemyPoppedTime   = 0;

// will check that:
// - all player units are still alive
// - recon team members are still alive
// - drop choppers have reached the exit point
GenericMissionConditions()
{
  var i, j, unitid;

  // does the player has anymore units?
  // TODO: is the comm building still alive?
  /*
  if (BattleGroupValid(BG_PLAYER) == 0)
  {
    PostMessage("Mission failed!");
    state = S_MISSION_FAILED;
    return;
  }
  */

  // check if the initial transport choppers have reached their destination 
  // and if so remove them from the map

  // check if there are no more hostile units in the comm bunker area
  // if so, start the countdown and send in the enemies
  if (metReconTeam == 0)
  {
      for(i = 0; i < GetBattleGroupUnitCount(BG_PLAYER); i++)
      {
        unitid = GetBattleGroupUnit(BG_PLAYER, i);

        if (dist2D(GetUnitLogicX(unitid), GetUnitLogicY(unitid), gReconLocation[0], gReconLocation[1]) < 5)
        {
	  PostMessage("Recon team contacted.");

	  if (bunkerAreaCleared == 0)
	  {
	    ShowMap(gCommLocation[0] - 4, gCommLocation[1] - 4, gCommLocation[0] + 4, gCommLocation[1] + 4, 2);
            MoveCamera(gCommLocation[0], gCommLocation[1]);

	    PostMessage("Start the assault!");
	  }

	  // change the battlegroup from recon to the player
	  while (GetBattleGroupUnitCount(BG_RECON) > 0)
	  {
	    ChangeUnitBattleGroup(GetBattleGroupUnit(BG_RECON, 0), BG_PLAYER);
	  }

          metReconTeam = 1;
	  break;
        }
      }
  }
    
  if (bunkerAreaCleared == 0)
  {
    var enemies = 0;

    for(i = gCommLocation[0] - 5; i < gCommLocation[0] + 5; i++)
    {
      for(j = gCommLocation[1] - 5; j < gCommLocation[1] + 5; j++)
      {
        var unitid = GetUnitAtPoint(i, j);

	// TODO: check the unit for infantry type
	if (GetPolitics(BG_PLAYER, GetUnitBattleGroup(unitid)) == 3 && IsUnitDead(unitid) != 1)
	{
	  enemies++;
	  break;
	}
      }
    }

    if (enemies == 0)
    {
      bunkerAreaCleared = 1;
      countdownFinished = 0;

      // send the recon team to the bunker
      missionCountdown = MissionTime();
      enemyPoppedTime  = missionCountdown;

      PostMessage("Hold the bunker for 2 minutes until the comm system is completely jammed.");

      // TODO: change the comm bunker bg to player's
    }
  }


  // countdown; start to drop hostile units that assault the bunker
  if (bunkerAreaCleared == 1 && countdownFinished == 0)
  {
    if (MissionTime() - missionCountdown < 120)
    {
      // insert 3 enemies every 10 seconds 
      // TODO: put different scripts for each enemy, it would give a more "realistic" touch
      if (MissionTime() - enemyPoppedTime > 5)
      {
        enemyPoppedTime = MissionTime();

        InsertInfantryAtPoint(0 + random(5), 30 + 5 - random(2), BG_ENEMIES, 0, "i-easyrecon-assault.csl");
        InsertInfantryAtPoint(0 + random(5), 30 + 5 - random(2), BG_ENEMIES, 0, "i-easyrecon-assault.csl");
        InsertInfantryAtPoint(0 + random(5), 30 + 5 - random(2), BG_ENEMIES, 0, "i-easyrecon-assault.csl");
      } 
    }
    else
    {
      // re-insert the two choppers and make them land to LZ
      countdownFinished = 1;
      InsertAircraftAtPoint(39, 20, BG_PLAYER_TRANSPORT, 2, "a-extraction.csl");
      InsertAircraftAtPoint(39, 22, BG_PLAYER_TRANSPORT, 2, "a-extraction.csl");
    }
  }

  // the mission is succesful if recon team and player units are within
  // a certain range of the extraction choppers
  if (countdownFinished == 1)
  {
    var extractionSuccesful = 1;

    for(i = 0; i < GetBattleGroupUnitCount(BG_PLAYER); i++)
    {
      unitid = GetBattleGroupUnit(BG_PLAYER, i);

      if (dist2D(GetUnitLogicX(unitid), GetUnitLogicY(unitid), gExtractLocation[0], gExtractLocation[1]) > 2)
      {
        extractionSuccesful = 0;
      }
    }

    if (extractionSuccesful == 1)
    {
      PostMessage("Mission Succesful!");
      state == S_MISSION_SUCCESFUL;
    }
  }
}

main()
{
  if (state == S_INIT)
  {
    Init();
  }

  if (state == S_TRANSPORT)
  {
    Transport();
  }

  if (state == S_BRIEFING_LAND)
  {
    BriefingLand();
  }

  if (state != S_MISSION_FAILED)
  {
    GenericMissionConditions();
  }
}