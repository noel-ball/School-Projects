class Start extends Scene {
    create() {
        this.engine.setTitle(this.engine.storyData.Title); // TODO: replace this text using this.engine.storyData to find the story title
        this.engine.show("Your work onboard a hostile alien research facility located on Saturn's Moon Titan.<br><br>")
        this.engine.show("You are the last surviving member of your crew after the latest subject breached containment.<br><br>");
        this.engine.show("You are safely locked away in the control center, with access to the facilities data-base terminal.<br><br>");
        this.engine.show("<br>");
        this.engine.gotoScene(Location, this.engine.storyData.InitialLocation);
    }
}

class Location extends Scene {
    create(key) {
        let locationData = this.engine.storyData.Locations[key]; // TODO: use `key` to get the data object for the current story location
        this.engine.show(locationData.Body); // TODO: replace this text by the Body of the location data
        if(key == "Login") {
            this.engine.show(nAttempts+" attempts left");
            if(Guess) { this.engine.show("Current Guess: "+Guess+"<br>"); }
        }

        if(Cronus == "E4" && key == "Cronus") { 
            if(Modules) { this.engine.show("The Network Link is accessible in this sector"); }
            else { this.engine.show("The Communications Tower is now accessible!") }
           
        }

        if(key == "Cronus" || key == "Repair") {
            let Cr = this.engine.mCoord(Cronus);
            let Ce = this.engine.mCoord(Cerberus);
            let L = this.engine.mCoord(Locked);
            this.engine.mUpdate(Cr, Ce, L);
        }

        if(key == "Repair") {
            this.engine.show("There are "+Modules+" broken modules remaining"); 
        }

        if(locationData.Guesses) {
            for(let guess of locationData.Guesses) {
                this.engine.addGuess(guess);
            }
        }

        if(locationData.Moves) {
            for(let move of locationData.Moves[Cronus]) {
                this.engine.addMoves(move.Text, move);
            }
        }

        if(locationData.Choices) { // TODO: check if the location has any Choices
            for(let choice of locationData.Choices) { // TODO: loop over the location's Choices
                this.engine.addChoice(choice.Text, choice); // TODO: use the Text of the choice
                // TODO: add a useful second argument to addChoice so that the current code of handleChoice below works
            }
        } else {
            this.engine.addChoice("The end.");
        }

        this.engine.show("<br>");
    }

    handleChoice(choice) {
        if(choice) {
            this.engine.show("&gt; "+choice.Text);
            while(this.engine.guessContainer.firstChild) {
                this.engine.guessContainer.removeChild(this.engine.guessContainer.firstChild)
            }
            if(choice.Target != "Cronus"){
                this.engine.gotoScene(Location, choice.Target);
            } else if (Guess == "Ungodly") {
                this.engine.gotoScene(MoveL, choice.Target)
            } else {
                nAttempts--;
                if(nAttempts == 0) {
                    this.engine.gotoScene(Location, "FailedLogin");
                } else {
                    this.engine.gotoScene(Location, "Login");
                }
            }
        } else {
            while(this.engine.guessContainer.firstChild) {
                this.engine.guessContainer.removeChild(this.engine.guessContainer.firstChild)
            }
            this.engine.gotoScene(End);
        }
    }

    handleGuess(guess) {
        Guess = guess;
        if(Guess) {
            this.engine.show("Current Guess: "+Guess);
        }
    }
}

class MoveL extends Location {
    handleMove(move) {
        if(move.Target == Cerberus){
            this.engine.show("&gt; You move: "+Cronus+" &#8594; "+move.Target);
            Cronus = move.Target;
            this.engine.gotoScene(Location, "Kill");
        } else if(move.Target){
            tLocked++;
            if(tLocked > 2) { Locked = "00"; }
            if(move.Target == Locked) {
                this.engine.show("&gt; That sector is locked");
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else { this.engine.gotoScene(MoveL, "Cronus"); }
            } else if(move.Target == "A4") { 
                if(Net) { 
                    this.engine.show("&gt; You move: "+Cronus+" &#8594; "+move.Target);
                    Cronus = move.Target;
                    this.engine.gotoScene(Location, "ComT"); 
                }
                else {
                    this.engine.show("&gt; The Communication Tower needs Network");
                    Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                    if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                    else { this.engine.gotoScene(MoveL, "Cronus"); }
                }
            } else {
                this.engine.show("&gt; You move: "+Cronus+" &#8594; "+move.Target);
                Cronus = move.Target;
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else { 
                    if(move.Target == "E4") { this.engine.gotoScene(NetL, "Cronus"); }
                    else { this.engine.gotoScene(MoveL, "Cronus"); }
                }
            } 
        } else if(move.Lock){
            if(Cerberus == move.Lock) {
                ("&gt; Cerberus keeps the gate to sector "+move.Lock+" open");
                this.engine.gotoScene(MoveL, "Cronus");
            }
            else {
                this.engine.show("&gt; You lock: "+Locked+" &#8594; "+move.Lock);
                Locked = move.Lock;
                tLocked = 0;
                this.engine.gotoScene(MoveL, "Cronus");
            }
        }
    }

    handleChoice(choice) {
        if(choice) {
            this.engine.show("&gt; "+choice.Text);
            while(this.engine.guessContainer.firstChild) {
                this.engine.guessContainer.removeChild(this.engine.guessContainer.firstChild)
            }
            if(choice.Target != "Repair" && choice.Target != "Fin") { this.engine.gotoScene(MoveL, choice.Target); } 
            else if (choice.Target == "Repair"){
                if(Cronus == "E4") { this.engine.gotoScene(NetL, choice.Target); }
                else {
                    this.engine.show("&gt; Nothing to repair");
                    this.engine.gotoScene(MoveL, "Cronus");
                }
            } else if (choice.Target == "Fin"){
                if(Net) { this.engine.gotoScene(Location, choice.Target); }
                else {
                    this.engine.show("&gt; Network is down");
                    this.engine.gotoScene(MoveL, "Cronus");
                }
            }
        } else {
            while(this.engine.guessContainer.firstChild) {
                this.engine.guessContainer.removeChild(this.engine.guessContainer.firstChild)
            }
            this.engine.gotoScene(End);
        }
    }
}

class NetL extends MoveL {
    handleMove(move) {
        if(move.Target){
            tLocked++;
            if(tLocked > 2) { Locked = "00"; }
            if(move.Target == Locked) {
                this.engine.show("&gt; That sector is locked");
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else { this.engine.gotoScene(MoveL, "Cronus"); }
            } else {
                this.engine.show("&gt; You move: "+Cronus+" &#8594; "+move.Target);
                Cronus = move.Target;
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else {
                    if(move.Target == "E4") { this.engine.gotoScene(NetL, "Cronus") }
                    else if(move.Target == "A4") { this.engine.gotoScene(Location, "ComT") }
                    else { this.engine.gotoScene(MoveL, "Cronus"); }
                }
            } 
        }
        if(move.Lock){
            if(Cerberus == move.Lock) {
                ("&gt; Cerberus keeps the gate to sector "+move.Lock+" open");
                this.engine.gotoScene(NetL, "Cronus");
            } else {
                this.engine.show("&gt; "+Locked+" &#8594; "+move.Lock);
                Locked = move.Lock;
                tLocked = 0;
                this.engine.gotoScene(MoveL, "Cronus");
            }
        }
    }

    handleChoice(choice) {
        if(choice) {
            this.engine.show("&gt; "+choice.Text);
            while(this.engine.guessContainer.firstChild) {
                this.engine.guessContainer.removeChild(this.engine.guessContainer.firstChild)
            }
            if(choice.Target != "Repair") { this.engine.gotoScene(NetL, choice.Target) } 
            else if (Cronus == "E4" && Modules > 1) { 
                Modules--;
                tLocked++;
                if(tLocked > 2) { Locked = "00"; }
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else { this.engine.gotoScene(NetL, choice.Target); }
            }
            else if (Cronus == "E4" && Modules == 1) {
                Modules--;
                tLocked++;
                if(tLocked > 2) { Locked = "00"; }
                Net++;
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else { this.engine.gotoScene(NetL, "Cronus"); }
            }
            else {
                this.engine.show("&gt; Nothing to repair");
                Cerberus = this.engine.CerberusTurn(Cronus, Cerberus, Locked);
                if(Cerberus == Cronus) { this.engine.gotoScene(Location, "Kill"); }
                else { this.engine.gotoScene(MoveL, "Cronus"); }
            }
        } else {
            while(this.engine.guessContainer.firstChild) {
                this.engine.guessContainer.removeChild(this.engine.guessContainer.firstChild)
            }
            this.engine.gotoScene(End);
        }
    }
}

class End extends Scene {
    create() {
        this.engine.show("<hr>");
        this.engine.show(this.engine.storyData.Credits);
    }
}

Engine.load(Start, 'myStory.json');

var Guess;
var nAttempts = 3;
var Cronus = "C1";
var Cerberus = "C5";
var Locked = "00";
var tLocked = 0;
var Net = 0;
var Modules = 3;
var map = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, "#", 1, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, "#", 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, "C", 1, 1, 1, "#", 1, 1, 1, "N", 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, "#", 1, 1, 1, 1, 1, 1, 1, "#", 1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
]