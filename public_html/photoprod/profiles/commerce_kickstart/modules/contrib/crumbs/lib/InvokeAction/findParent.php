<?php


class crumbs_InvokeAction_findParent extends crumbs_InvokeAction_findForPath {

  protected $method = 'findParent';

  protected function _invoke($plugin, $method) {
    $result = $plugin->$method($this->path, $this->item);
    return $result;
  }
}
