#include "ast_helper_print.h"

#include "custom_queue_helpers.h"

#include <stdio.h>

static void print_node(const ast_display_node_t *node);

void ast_helpers_print_tree(ast_node_t *tree_root)
{
  ret_code_t            ret_code = RET_CODE_OK;
  custom_queue_t        stack = TAILQ_HEAD_INITIALIZER(stack);
  ast_display_node_t    display_node;
  ast_display_node_t   *node;
  ast_node_t           *ast_node_to_store;

  if (NULL == tree_root)
  {
    return;
  }

  custom_queue_empty(&stack);

  puts("===================");

  display_node.ast_node = tree_root;
  display_node.level    = 0;

  ret_code = custom_queue_helper_insert_ast_display_node(
      &stack,
      &display_node,
      TAIL_OR_LAST);

  while (!TAILQ_EMPTY(&stack) && RET_CODE_OK == ret_code)
  {
    ret_code = custom_queue_pop(
        &stack,
        (void **)&node,
        TAIL_OR_LAST);

    if (RET_CODE_OK == ret_code)
    {
      print_node(node);

      display_node.level = node->level + 1;

      ast_node_to_store = node->ast_node->right;
      if (NULL != ast_node_to_store)
      {
        display_node.ast_node = ast_node_to_store;

        ret_code = custom_queue_helper_insert_ast_display_node(
            &stack,
            &display_node,
            TAIL_OR_LAST);
      }
    }
    if (RET_CODE_OK == ret_code)
    {
      ast_node_to_store = node->ast_node->left;
      if (NULL != ast_node_to_store)
      {
        display_node.ast_node = ast_node_to_store;

        ret_code = custom_queue_helper_insert_ast_display_node(
            &stack,
            &display_node,
            TAIL_OR_LAST);
      }
    }
  }

  puts("\\==================");
  custom_queue_empty(&stack);
}

static void print_node(const ast_display_node_t *display_node)
{
  if (NULL == display_node)
  {
    return;
  }

  int level = display_node->level;

  while (level > 0)
  {
    printf("|  ");
    level--;
  }
  printf("[");
  token_print(&(display_node->ast_node->token));
  puts("");
}

