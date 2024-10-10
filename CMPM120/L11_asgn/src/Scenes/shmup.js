class shmup extends Phaser.Scene {
    keyA;
    keyD;
    keySpace;

    constructor() {
        super('shmup');
        this.sprite = {sprite: {}};
    }

    preload() {
        // Assets from Kenny Assets pack "Pixel Platformer" and "Cursor Pack"
        // https://kenney.nl/assets/pixel-platformer
        // https://kenney.nl/assets/cursor-pack
        this.load.setPath('./assets/');
        this.load.image('char', 'tile_0003.png');
        this.load.image('bullet', 'pointer_scifi_b.png');
    }

    create() {
        let sp = this.sprite; // alias for readability
        // create char and movement
        sp.char = this.add.sprite(400, 550, 'char', 'tile_0003.png');
        sp.char.setScale(2, 2);
        this.keyA = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A);
        this.keyD = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D);
        // create projectile
        sp.bullet = this.add.sprite(400, 550, 'bullet', 'pointer_scifi_b.png');
        sp.bullet.setAngle(45);
        sp.bullet.visible = false;
        this.keySpace = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE); 
        this.active = false;
    }

    update() {
        let sp = this.sprite; // alias for readability 
        
        if(this.keyA.isDown) {
            sp.char.x -= 10;
            this.facing = 0;
        }
        if(this.keyD.isDown) {
            sp.char.x += 10;
            this.facing = 1;            
        }
        if(this.keySpace.isDown && !sp.bullet.visible) {
            sp.bullet.setPosition(sp.char.x, sp.char.y);
            sp.bullet.visible = true;
        }
        if(sp.bullet.visible){
            sp.bullet.y -= 25;
        }
        if(!(sp.bullet.x >= 0 && sp.bullet.x <= 800 && sp.bullet.y >= 0 && sp.bullet.y <= 600)){
            sp.bullet.visible = false;
        }
    }
}