<?php


function crumbs_render_breadcrumb_item(array $item) {
  $options = $item['link_options'];
  return l($item['link_title'], $item['link_path'], $options);
}


function crumbs_build_breadcrumb(array $trail) {
  $pluginEngine = _crumbs_load_plugin_engine();
  $breadcrumbBuilder = new crumbs_BreadcrumbBuilder($pluginEngine);
  return $breadcrumbBuilder->buildBreadcrumb($trail);
}



