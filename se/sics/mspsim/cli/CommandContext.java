package se.sics.mspsim.cli;
import java.io.PrintStream;
import se.sics.mspsim.util.MapTable;

public class CommandContext {

  private String[] args;
  private String commandLine;
  private MapTable mapTable;
  private int pid = -1;
  private Command command;
  
  public PrintStream out;
  public PrintStream err;
  
  public CommandContext(MapTable table, String commandLine, String[] args,
			int pid, Command command, PrintStream out, PrintStream err) {
    this(table, commandLine, args, pid, command);
    setOutput(out, err);
  }
  
  public CommandContext(MapTable table, String commandLine, String[] args,
			int pid, Command command) {
    this.commandLine = commandLine;
    this.args = args;
    this.pid = pid;
    this.mapTable = table;
    this.command = command;
  }
  
  void setOutput(PrintStream out, PrintStream err) {
    this.out = out;
    this.err = err;
  }
  
  Command getCommand( ) {
    return command;
  }
  
  String getCommandLine() {
    return commandLine;
  }

  public int getPID() {
    return pid;
  }

  /**
   * exit needs to be called as soon as the command is completed (or stopped).
   * @param exitCode - the exit code of the command
   */
  public void exit(int exitCode) {
    // TODO: Clean up can be done now!
    pid = -1;
  }

  public MapTable getMapTable() {
    return mapTable;
  }

  public String getCommandName() {
    return args[0];
  }

  public int getArgumentCount() {
    return args.length - 1;
  }
  
  public String getArgument(int index) {
    return args[index + 1];
  }
  
  public int getArgumentAsAddress(int index) {
    String adr = getArgument(index);
    if (adr == null || adr.length() == 0) return 0;
    adr = adr.trim();
    if (adr.charAt(0) == '$') {
      try {
        return Integer.parseInt(adr.substring(1), 16);
      } catch (Exception e) {
        err.println("Illegal hex number format: " + adr);
      }
    } else if (Character.isDigit(adr.charAt(0))) {
      try {
        return Integer.parseInt(adr); 
      } catch (Exception e) {
        err.println("Illegal number format: " + adr);
      }
    } else {
      // Assume that it is a symbol
      if (mapTable != null) {
        return mapTable.getFunctionAddress(adr);
      }
    }
    return 0;
  }

  public int getArgumentAsInt(int i) {
    try {
      return Integer.parseInt(getArgument(i));
    } catch (Exception e) {
      err.println("Illegal number format: " + getArgument(i));
    }
    return 0;
  }

  public String toString() {
    return (pid >= 0 ? ("" + pid) : "?") + '\t' + (commandLine == null ? getCommandName() : commandLine);
  }

}
