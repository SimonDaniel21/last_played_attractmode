Simple Program to create an "last-played" playlist inside attract-mode from starting games since the script at http://forum.attractmode.org/index.php?topic=1843.0 did not work for me.
in your layout.nut:
ADD
fe.plugin_command_bg( "path-to-playlistCreator", [path to rom list] [max num of games] fe.game_info(Info.Name) + " " + fe.game_info(Info.Title) + " " + fe.game_info(Info.Emulator));
INSIDE
function on_transition( ttype, var, ttime )
{
	if(ttype == Transition.ToGame)
		[HERE]
}

usage:
./playlistCreator [path to rom list] [max num of games] [title] [name] [emulator]
