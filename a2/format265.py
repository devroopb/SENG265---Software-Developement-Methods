import fileinput

# Global opt
opt = { 
    '.FT'	: False      # Is formatting on or off?
    '.LW'	: 0          # The Maximum width of any given line. May change throughout processing.
    '.LM'	: 0          # Desired page margin. Cannot be less then or equal to .LW-20
    '.LS'	: 0
}

state = {
    'line_length'	: 0
    'is_different'	: False
}

def main ():
    """
    Format according to given control sequences.
    """
    # Define our variables.
    # The currently calculated output.
    out = ""

    # Process each line. If we detect a control sequence on the line, return none. If we detect [], add an extra newline. Otherwise just check for paragraph size and margin. The output will be joined with 'out'
    lines =  [ line for line in fileinput.input() ]
    processed = [ process_line(line) for line in lines]
    not_empty = [ line for line in processed if line != None ]
    
    if len(not_empty) == 0:
        return
    else:
        out = "".join(not_empty)
        print out,
    
def process_line (line):
    """ 
    Process the line. Check for control sequences, set any flags required. Format appropriately.
    """
    global opt, state

    split = line.split()
    # Detect control sequences.
    if len(split) is not 0:
        if split[0] == ".FT":
            if split[1] == "off":
                opt['.FT'] = False
            elif split[1] == "on":
                opt['.FT'] = True
            return None
        elif split[0] == ".LW": # Also turns on .FT
            if isinstance(int(split[1]), int):
                opt['.LW'] = int(split[1])
                opt['.FT'] = True
            elif split[1][0] == '-':
                opt['.LW'] -= int(split[1][1:])
                opt['.FT'] = True
            elif split[1][0] == '+':
                opt['.LW'] += int(split[1][1:])
                opt['.FT'] = True
            return None
        elif split[0] == ".LM":
            if split[1][:1] == "-":
                opt['.LM'] -= int(split[1][1:])
                if opt['.LM'] < 0:
                    opt['.LM'] = 0
            elif split[1][:1] == "+":
                opt['.LM'] += int(split[1][1:])
            else:
                opt['.LM'] = int(split[1])
            # Restriction on margins.
            if opt['.LM'] > opt['.LW'] - 20:
                opt['.LM'] = opt['.LW'] - 20
            return None
            
    # output the line
    if opt['.FT']:
        # Special case for empty lines.
        if split == []:
            state['line_length'] = 0
            if state['is_different']:
                return '\n'
            else:
                state['is_different'] = True
                return '\n\n'
        state['is_different'] = False
        
        # Do we need a margin? (Is this a new line?)
        if state['line_length'] == 0:
            out = "".join( [" " for i in range(opt['.LM']) ] )
            state['line_length'] = opt['.LM']
        else:
            out = ""
            
        # Gradually add each word, making sure we're not going over the .LW
        for word in split:
            # Do we need a new line?
            if state['line_length'] + len(word) >= opt['.LW']:
                margin = "".join( [" " for i in range(opt['.LM']) ] )
                out = out + '\n' + margin
                state['line_length'] = opt['.LM']
            # Otherwise, add a space
            elif state['line_length'] != opt['.LM']:
                state['line_length'] += 1
                out = out + ' '
            # Concatenate
            out = out + word
            state['line_length'] += len(word)
        return out
    else:
        return line
    
# Detect if we're running via ./ or if we're called.
if __name__ == "__main__":
    main()