// class PresetService {
// private:
//     db::Table<dm::Preset> _presetTable;
//     db::Table<dm::PresetResource> _junctionTable;
//     ResourceService& _resService;

// public:
//     PresetService(db::Database& db, ResourceService& resService) 
//         : _resService(resService) 
//     {
//         _presetTable = db.open_table<dm::Preset>("presets");
//         _junctionTable = db.open_table<dm::PresetResource>("preset_resources");
//     }

//     bool launchPreset(uint64_t presetId) {
//         auto links = _junctionTable.get_all();
//         bool found = false;
//         for (const auto& link : links) {
//             if (link.preset_id == presetId) {
//                 _resService.launchResource(link.resource_id);
//                 found = true;
//             }
//         }
//         return found;
//     }
// };