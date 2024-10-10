class Load extends Phaser.Scene {
    constructor() {
        super("loadScene");
    }

    preload() {
        this.load.setPath("./assets/");

        // Load characters spritesheet
        this.load.atlas('knight', 'Knight/knightWalk.png', 'Knight/knightWalk.json');
        this.load.atlas('heavyKnight', 'heavyKnight/heavyKnightWalk.png', 'heavyKnight/heavyKnightWalk.json');
 
        // Load enemy spritesheet
        this.load.image('spirit', 'Spirits/spirit.png');
        this.load.image('evilSpirit', 'Spirits/evilSpirit.png');

        // Load tilemap information
        this.load.image("rock_packed", "Tiled/rock_packed.png");
        this.load.image("tilemap_packed_dung", "Tiled/tilemap_packed_dung.png");
        this.load.image("tilemap_packed_plat", "Tiled/tilemap_packed_plat.png");
        this.load.spritesheet("plat_tiles", "Tiled/tilemap_packed_plat.png", {frameWidth: 18, frameHeight: 18});
        this.load.spritesheet("dung_tiles", "Tiled/tilemap_packed_dung.png", {frameWidth: 16, frameHeight: 16});
        // this.load.tilemapTiledJSON("HUD", "HUD.tmj"); 
        this.load.tilemapTiledJSON("Level1", "Tiled/Level1.tmj");
        this.load.tilemapTiledJSON("Boss1", "Tiled/Boss1.tmj");
    }

    create() {
        this.anims.create({
            key: 'knight',
            frames: this.anims.generateFrameNames('knight', {
                prefix: "knight-",
                start: 1,
                end: 2,
                suffix: ".png",
                zeroPad: 1
            }),
            frameRate: 3,
            repeat: -1
        });

        this.anims.create({
            key: 'heavyKnight',
            frames: this.anims.generateFrameNames('heavyKnight', {
                prefix: "heavyKnight-",
                start: 1,
                end: 2,
                suffix: ".png",
                zeroPad: 1
            }),
            frameRate: 3,
            repeat: -1
        });

         // ...and pass to the next Scene
         this.scene.start("AdventureScene");
    }

    // Never get here since a new scene is started in create()
    update() {
    }
}