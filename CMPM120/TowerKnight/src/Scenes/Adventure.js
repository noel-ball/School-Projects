class Adventure extends Phaser.Scene {
    constructor() {
        super("AdventureScene");
    }

    init() {
        // variables and settings
        this.playerVelocity = {
            acceleration: 12*16,
            maxSpeed: 0,
            deceleration: -22*16,
            jumpHeight: -8*16,
            gravity: 8*16,
            jumpDuration: 3.6
        }
        this.key1 = false;
        this.active = true;
    }

    create() {
        cursors = this.input.keyboard.createCursorKeys();
        this.spaceBar = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE)

//CREATING MAP/TILESETS===================================================================================================================
        this.map = this.add.tilemap("Level1");
        this.backgrounds = this.map.addTilesetImage("Backgrounds", "tilemap_packed_dung");
        this.platforms = this.map.addTilesetImage("Platforms","rock_packed");
        this.extras = this.map.addTilesetImage("Extras", "tilemap_packed_plat");

        this.backgroundLayer = this.map.createLayer("Backdrop", [this.backgrounds], 0, 0);
        this.backgroundLayer.setTint(0x888888) // Apply a gray tint to lower brightness
        this.groundLayer = this.map.createLayer("TileLayer1", [this.platforms, this.extras], 0, 0);
        this.groundLayer.setCollisionByProperty({//collision with geometry layer
            collides: true
        });

        this.physics.world.setBounds(0, 0, this.map.widthInPixels, this.map.heightInPixels);

//PLAYER SETUP============================================================================================================================
        my.sprite.player = this.physics.add.sprite(112, 760, "knight", "knight-1.png").setDepth(100); 
        my.sprite.player.armor = 'knight';
        my.sprite.player.grounded = false;
        this.physics.world.enable(my.sprite.player);
        my.sprite.player.body.setCollideWorldBounds(true);
        this.physics.add.collider(my.sprite.player, this.groundLayer);
        this.physics.world.gravity.y = 150;

        this.groundLayer.setTileIndexCallback(282, (sprite, tile) => {
            if(tile && tile.index == 282) {this.scene.restart();}
        }, this);

//ITEMS====================================================================================================================================
        my.sprite.key1 = this.add.sprite(14, 238, "plat_tiles", 27);
        my.sprite.lock1 = this.add.sprite(192, 24, "plat_tiles", 28);
        my.sprite.closedL = this.add.sprite(208, 24, "dung_tiles", 46);
        my.sprite.closedR = this.add.sprite(224, 24, "dung_tiles", 47);

//DEUBG====================================================================================================================================
        this.input.keyboard.on('keydown-R', () => {
            if (!this.active) {
                this.scene.restart();
            }
        })

//CAMERA===================================================================================================================================
        // adjust camera to full game canvas
        this.mapCamera = this.cameras.main
        this.mapCamera.setViewport(0, 0, 240, 266);
        this.mapCamera.setBounds(0, 0, this.map.widthInPixels, this.map.heightInPixels);
        this.mapCamera.startFollow(my.sprite.player);
       }

//MISC FUNCTIONS=========================================================================================================================
    
    //sprite collision
    collides(a, b) {
        if (Math.abs(a.x - b.x) > (a.displayWidth / 2 + b.displayWidth / 2 - 10))return false;
        if (Math.abs(a.y - b.y) > (a.displayHeight + b.displayHeight - 10)) return false;
        return true;
    }
    
    update() {
        if (this.active) {
            // correct grounded from constantly moving the player.
            let tile = this.map.getTileAtWorldXY(my.sprite.player.x, my.sprite.player.y + 16, true, this.mapCamera, this.groundLayer);
            if(tile && tile.index != -1) {my.sprite.player.grounded = true; this.physics.world.gravity.y = 0;}
            else if (tile.index == -1) {my.sprite.player.grounded = false; this.physics.world.gravity.y = 150;}

        
//PLAYER CHECKS=========================================================================================================================
            let anim = my.sprite.player.armor;

            // correct max velocity
            if(my.sprite.player.velocity > my.playerVal.maxSpeed) {
                my.sprite.player.setVelocityX(my.playerVal.maxSpeed);
            }
            
            // player movement
            if(cursors.left.isDown) { //move left pressed
                my.sprite.player.setAccelerationX(-this.playerVelocity.acceleration);
                my.sprite.player.anims.play(anim, true);
                my.sprite.player.setFlip(true, false);
            } else if(cursors.right.isDown) { //move right pressed
                my.sprite.player.setAccelerationX(this.playerVelocity.acceleration);
                my.sprite.player.anims.play(anim, true);
                my.sprite.player.resetFlip();    
            } else { // no longer walking, enact decel
                my.sprite.player.setAccelerationX(0);
                my.sprite.player.setDragX(18*16);
            }
            if (Phaser.Input.Keyboard.JustDown(this.spaceBar) && my.sprite.player.grounded) { //jump pressed
                my.sprite.player.setVelocityY(this.playerVelocity.jumpHeight);
                // console.log("v:"+my.sprite.player.body.velocity.x+","+my.sprite.player.body.velocity.y);
            }

            // key progression
            if (this.collides(my.sprite.player, my.sprite.key1)) {
                my.sprite.key1.destroy();
                this.key1 = true;
            }
            if (this.collides(my.sprite.player, my.sprite.lock1) && this.key1 === true) {
                my.sprite.lock1.destroy();
                my.sprite.closedL.destroy();
                my.sprite.closedR.destroy();
                my.sprite.openL = this.add.sprite(208, 24, "dung_tiles", 10);
                my.sprite.openR = this.add.sprite(224, 24, "dung_tiles", 11);
            }
            
            // end of game
            if (my.sprite.openR) {
                if (this.collides(my.sprite.player, my.sprite.openR)) {
                    this.active = false;
                    this.add.text(80, 32, 'You win!', {
                        font: '18px PressStart2p',
                        fill: '#000000'
                    })
                    this.add.text(48, 48, 'Press R to restart', {
                        font: '18px PressStart2p',
                        fill: '#000000'
                    })
                }
            }
        }
    }
}
//TIMERS=================================================================================================================================
        // this.gameFrame++; 6