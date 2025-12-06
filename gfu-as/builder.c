#include <gamefu/as/as.h>

void gfuas_builder_position_at_start(gfuas_builder* b) {
    if (b == nullptr) return;
    b->current = nullptr;
}

void gfuas_builder_position_at_end(gfuas_builder* b) {
    if (b == nullptr) return;
    b->current = b->tail;
}

void gfuas_builder_position_before(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    b->current = stmt->prev;
}

void gfuas_builder_position_after(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    b->current = stmt;
}

void gfuas_builder_insert(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    if (b->head == nullptr) {
        assertn(b->current == nullptr);
        assertn(b->tail == nullptr);
        b->head = b->tail = stmt;
    } else if (b->current == nullptr) {
        assertn(b->head != nullptr);
        stmt->next = b->head;
        b->head = stmt;
    } else {
        assertn(b->current != nullptr);
        b->current->prev->next = stmt;
        stmt->prev = b->current->prev;
        b->current->prev = stmt;
        stmt->next = b->current;
        b->current = stmt;
    }
}
