#include "table.h"

void table_alloc(Table *table, int bits) {
	table->size = 1 << bits;
	table->mask = table->size - 1;
	table->entry = calloc(table->size, sizeof(Entry));
}

void table_free(Table *table) {
	free(table->entry);
}

Move *table_get_move(Table *table, bb key) {
	Entry *entry = TABLE_ENTRY(table, key);
	if (entry->key == key) {
		return &entry->move;
	}

	return NULL;
}

void table_set_move(Table *table, bb key, int depth, Move *move) {
	Entry *entry = TABLE_ENTRY(table, key);
	if (entry->depth <= depth) {
		entry->key = key;
		entry->depth = depth;
		memcpy(&entry->move, move, sizeof(Move));
	}
}

void table_set(Table *table, bb key, int depth, int value, int flag) {
	Entry *entry = TABLE_ENTRY(table, key);
	if (entry->depth <= depth) {
		entry->key 	 = key;
		entry->depth = depth;
		entry->score = value;
		entry->flag  = flag;
	}

}

int table_get(Table *table, bb key, int depth, int alpha, int beta, int *value) {
	Entry *entry = TABLE_ENTRY(table, key);
	int flag = 0;
	if (entry->key == key) {
		if (entry->depth >= depth) {
			if (entry->flag == EXACT) {
				*value = entry->score;
				flag = 1;
			}
			if ((entry->flag == ALPHA) && (entry->score <= alpha)) {
				*value = alpha;
				flag = 1;
			}
			if ((entry->flag == BETA) && (entry->score >= beta)) {
				*value = beta;
				flag = 1;
			}
		}
	}
	return flag;
}
