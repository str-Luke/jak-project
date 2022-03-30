#pragma once

#include <optional>

#include "game/graphics/gfx.h"
#include "game/graphics/opengl_renderer/background/background_common.h"
#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "game/graphics/pipelines/opengl.h"
#include "common/util/FilteredValue.h"

class Shrub : public BucketRenderer {
 public:
  Shrub(const std::string& name, BucketId my_id);
  ~Shrub();
  bool setup_for_level(const std::string& level, SharedRenderState* render_state);
  void render_all_trees(const TfragRenderSettings& settings,
                        SharedRenderState* render_state,
                        ScopedProfilerNode& prof);
  void render_tree(int idx,
                   const TfragRenderSettings& settings,
                   SharedRenderState* render_state,
                   ScopedProfilerNode& prof);
  void render(DmaFollower& dma, SharedRenderState* render_state, ScopedProfilerNode& prof) override;
  void draw_debug_window() override;

 private:
  void update_load(const Loader::LevelData* loader_data);
  void discard_tree_cache();

  struct Tree {
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint time_of_day_texture;
    std::vector<u32> index_list;
    GLuint vao;
    u32 vert_count;
    const std::vector<tfrag3::ShrubDraw>* draws = nullptr;
    const std::vector<tfrag3::TieWindInstance>* instance_info = nullptr;
    const std::vector<tfrag3::TimeOfDayColor>* colors = nullptr;
    SwizzledTimeOfDay tod_cache;

    std::vector<std::array<math::Vector4f, 4>> wind_matrix_cache;

    bool has_wind = false;
    GLuint wind_vertex_index_buffer;
    std::vector<u32> wind_vertex_index_offsets;

    struct {
      u32 index_upload = 0;
      u32 verts = 0;
      u32 draws = 0;
      u32 full_draws = 0;  // ones that have all visible
      u32 wind_draws = 0;
      Filtered<float> cull_time;
      Filtered<float> index_time;
      Filtered<float> tod_time;
      Filtered<float> setup_time;
      Filtered<float> draw_time;
      Filtered<float> tree_time;
    } perf;
  };

  std::vector<Tree> m_trees;
  std::string m_level_name;
  const std::vector<GLuint>* m_textures;
  u64 m_load_id = -1;

  std::vector<math::Vector<u8, 4>> m_color_result;

  static constexpr int TIME_OF_DAY_COLOR_COUNT = 8192;
  bool m_has_level = false;

  struct Cache {
    std::vector<std::pair<int, int>> draw_idx_temp;
  } m_cache;
  TfragPcPortData m_pc_port_data;
};
