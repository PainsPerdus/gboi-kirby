use serde::Deserialize;

#[derive(Deserialize, Clone, Copy)]
pub struct LevelValues {
    pub top_door: bool,
    pub left_door: bool,
    pub corner: bool,
    pub clutter_level: u8,
}

#[derive(Deserialize)]
pub struct Level {
    pub layers: (BackgroundLayer, EntityLayer), // XXX: fragile... is the layer ordering guarantteed?,
    pub values: LevelValues,
}

#[derive(Deserialize)]
pub struct EntityLayer {
    pub name: String,
    #[serde(alias = "_eid")]
    pub eid: String,
    #[serde(alias = "offsetX")]
    pub offset_x: Option<i32>,
    #[serde(alias = "offsetY")]
    pub offset_y: Option<i32>,
    #[serde(alias = "gridCellWidth")]
    pub grid_cell_width: i32,
    #[serde(alias = "gridCellHeight")]
    pub grid_cell_height: i32,
    #[serde(alias = "gridCellsX")]
    pub grid_cells_x: i32,
    #[serde(alias = "gridCellsY")]
    pub grid_cells_y: i32,

    pub entities: Vec<Entity>,
}

#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct BackgroundLayer {
    pub name: String,
    #[serde(alias = "_eid")]
    pub eid: String,
    #[serde(alias = "offsetX")]
    pub offset_x: Option<i32>,
    #[serde(alias = "offsetY")]
    pub offset_y: Option<i32>,
    #[serde(alias = "gridCellWidth")]
    pub grid_cell_width: i32,
    #[serde(alias = "gridCellHeight")]
    pub grid_cell_height: i32,
    #[serde(alias = "gridCellsX")]
    pub grid_cells_x: i32,
    #[serde(alias = "gridCellsY")]
    pub grid_cells_y: i32,

    pub tileset: String,
    #[serde(alias = "data")]
    pub data: Vec<i32>,
}

#[derive(Deserialize)]
pub struct EntityValues {
    #[serde(alias = "Id")]
    pub id: u8,
}

#[derive(Deserialize)]
pub struct Entity {
    pub name: String,
    pub x: i32,
    pub y: i32,
    #[serde(alias = "originX")]
    pub origin_x: Option<i32>,
    #[serde(alias = "originY")]
    pub origin_y: Option<i32>,
    pub nodes: Option<Vec<Position>>,
    pub rotation: Option<f32>,
    pub values: EntityValues,
}

#[derive(Deserialize)]
pub struct Position {
    pub x: i32,
    pub y: i32,
}
