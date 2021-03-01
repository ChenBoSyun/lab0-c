#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q == NULL) {
        printf("Failed to malloc queue_t\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}



/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL) {
        return;
    }
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    list_ele_t *curr = q->head;
    list_ele_t *tmp;
    while (curr != NULL) {
        tmp = curr->next;
        free(curr->value);
        free(curr);
        curr = tmp;
    }
    free(q);
    return;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    if (q == NULL) {
        return false;
    }

    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }

    memcpy(newh->value, s, strlen(s));
    *(newh->value + strlen(s)) = '\0';
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    q->head = newh;
    q->size++;
    if (q->size == 1) {
        q->tail = newh;
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL) {
        return false;
    }

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) {
        return false;
    }

    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newt->value == NULL) {
        free(newt);
        return false;
    }

    memcpy(newt->value, s, strlen(s));
    *(newt->value + strlen(s)) = '\0';
    newt->next = NULL;

    q->tail->next = newt;
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL) {
        return false;
    }
    char *str = q->head->value;

    if (sp != NULL) {
        if (strlen(str) > bufsize - 1) {
            memcpy(sp, str, bufsize - 1);
            *(sp + bufsize - 1) = '\0';
        } else {
            memcpy(sp, str, strlen(str));
            *(sp + strlen(str)) = '\0';
        }
    }
    free(str);
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL || q->head == NULL) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->head == NULL) {
        return;
    }
    q->tail = q->head;

    list_ele_t *prev = NULL;
    list_ele_t *curr = q->head;
    list_ele_t *next;
    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
    return;
}


list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }
    if (*(left->value) < *(right->value)) {
        left->next = merge(left->next, right);
        return left;
    } else {
        right->next = merge(left, right->next);
        return right;
    }
}

list_ele_t *merge_list(list_ele_t *head)
{
    if (head == NULL || head->next == NULL) {
        return head;
    }
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *left = merge_list(head);
    list_ele_t *right = merge_list(fast);

    return merge(left, right);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->head == NULL) {
        return;
    }

    q->head = merge_list(q->head);
    list_ele_t *curr = q->head;
    while (curr && curr->next) {
        curr = curr->next;
    }
    q->tail = curr;
    return;

    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
